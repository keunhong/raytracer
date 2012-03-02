/**
 * @file
 * @author  Keunhong Park <park282@illinois.edu>
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The camera class represents a (pinhole) camera
 */
 

#include "camera.hpp"

Camera::Camera(){}
Camera::Camera(Scene *scene_, Vec3 optical_center, int width_, int height_, double fovx_) : width(width_), height(height_), fovx(fovx_){
    fovy = height/width * fovx;
    focal_length = height / (2*std::tan(fovx/2));

    focal_point.x = optical_center.x;
    focal_point.y = optical_center.y;
    focal_point.z = optical_center.z - focal_length; // negative since -z is into the screen
    this->optical_center = optical_center;

    scene = scene_;

    std::cout << "Camera: focal_point=" << focal_point 
              << ", optical_center=" << optical_center
              << ", res=" << width << "x" << height
              << ", f=" << focal_length
              << std::endl;
}

int Camera::get_width() const{
    return width;
}
int Camera::get_height() const{
    return height;
}

/**
 * Traces the entire scene to an OpenCV image
 *
 * @param  im an opencv image
 */
#define NUM_LENSE_SAMPLES 1.0
void Camera::render(cv::Mat &im, int rays_per_pixel, double exposure, double time_step) const{

    double aperture_size = 6;
    Vec3 lense_center(focal_point.x, focal_point.y, focal_point.z+focal_length*2.0);

    Color *image = new Color[height*width];

    // time loop
    for(double t = 0.0; t < exposure; t += time_step){
        // x loop
        for(int u = 0; u < width; u++){
            if(u%10==0) cout << (double)(t/time_step*width+u)/(width*exposure/time_step)*100.0 << "%" << endl;
            // y loop
            for(int v = 0; v < height; v++){
                Color color;
                double x = u - width/2;
                double y = v - height/2;
                for(int ri = 0; ri < rays_per_pixel; ri++){
                    x += 1.0/rays_per_pixel;
                    y += 1.0/rays_per_pixel;

                    Color lense_color;
                    Vec3 image_point(focal_point.x+x, focal_point.y+y, focal_point.z);
                    for(int i = 0; i < NUM_LENSE_SAMPLES; i++){
                        for(int j = 0; j < NUM_LENSE_SAMPLES; j++){
                            double lense_x = i*aperture_size/NUM_LENSE_SAMPLES - aperture_size/2;
                            double lense_y = j*aperture_size/NUM_LENSE_SAMPLES - aperture_size/2;

                            Vec3 lense_point(focal_point.x+lense_x, focal_point.y+lense_y, focal_point.z+focal_length*2.0);

                            Vec3 ray_start = lense_point;
                            Vec3 ray_direction = image_point-lense_point;
                            Ray ray(ray_start, ray_direction.normalize(), NULL);
                            Trace trace = trace_ray(ray, 1.0, 0);
                            lense_color.add(trace.color);
                        }
                    }
                    lense_color = lense_color / (NUM_LENSE_SAMPLES*NUM_LENSE_SAMPLES);
                    color = color + lense_color;
                }
                
                color = color/rays_per_pixel;

                // Image will be inverted
                image[v*width+u].b += color.b/(exposure/time_step);
                image[v*width+u].g += color.g/(exposure/time_step);
                image[v*width+u].r += color.r/(exposure/time_step);
            }
        }
        scene->tick(time_step);
    }

    for(int u = 0; u < width; u++){
        for(int v = 0; v < height; v++){
            im.at<cv::Vec3b>(height-1-v,u)[0] = std::min(255.0,image[v*width+u].b * 255);
            im.at<cv::Vec3b>(height-1-v,u)[1] = std::min(255.0,image[v*width+u].g * 255);
            im.at<cv::Vec3b>(height-1-v,u)[2] = std::min(255.0,image[v*width+u].r * 255);
        }
    }
    delete [] image;
}



/**
 * traces from a point $p$ in the given direction v
 *
 * v is automatically normalized
 *
 * @param  p the starting point of the ray
 * @param  v_ the unnormalized direction of the ray
 * @param rho the current albedo
 * @return the result of the trace holding the color
 */
#define MIN_ALBEDO 1.0/255.0
Trace Camera::trace_ray(const Ray &ray, double rho, unsigned int depth) const{
    // Prune
    if(rho < MIN_ALBEDO){
        //cout << "Pruning trace tree since albedo is negligible." << rho << endl;
        return Trace(Color::MAGENTA, TRACE_EMPTY, NULL);
    }

    Intersection intersection = find_nearest_intersection(ray);

    // If no intersectin
    if(intersection.result == 0){
        return Trace(Color::MAGENTA, TRACE_EMPTY, NULL);
    }

    if(intersection.primitive->is_luminaire()){
        return Trace(Color::WHITE, TRACE_VALID, intersection.primitive);
    }

    const Material *material = intersection.primitive->get_material();

    /*
     * LOCAL
     */
    // Ambient
    Color local_intensity(0.1, 0.1, 0.1, 1.0);

    // Only process if intersected from outside of object
    if(intersection.result == 1){
        // Iterate through light sources for shadow rays
        for(vector<Primitive*>::const_iterator it = scene->luminaires.begin(); it != scene->luminaires.end(); ++it){
            Primitive *luminaire = *it;
            vector<Vec3> *samples = luminaire->get_samples();
            Color radiosity_gathered(0,0,0);

            for(vector<Vec3>::iterator jt = samples->begin(); jt != samples->end(); jt++){
                Vec3 dir = *jt - intersection.position;

                // Shoot shadow ray
                Ray shadow_ray(intersection.position, dir, intersection.primitive);
                Intersection shadow_intersection = find_nearest_intersection(shadow_ray); 

                if(!(shadow_intersection.result == 1 && !shadow_intersection.primitive->is_luminaire())){
                    // Foreshortening factors
                    // Doesn't work because we're not using a physical model of light (just yet)
                    // TODO: figure out later
                    /*double cos_theta_i = (dir*intersection.normal)/(dir.length()*intersection.normal.length());
                    double cos_theta_s = ((-dir)*shadow_intersection.normal)/((-dir).length()*shadow_intersection.normal.length());
                    double dist = (shadow_intersection.position - intersection.position).length();
                    double foreshortening_factor = cos_theta_i * cos_theta_s / (M_PI*dist*dist);*/

                    //std::cout << cos_theta_i << " " << cos_theta_s << " " << dist  << " " << foreshortening_factor << std::endl;

                    // Diffuse
                    Color intensity = luminaire->get_exitance() * material->rho_d;
                    double ndotl = dir.normalize()*intersection.normal;
                    ndotl = std::max(0.0, ndotl);
                    radiosity_gathered.add(intensity * ndotl);
                    radiosity_gathered = radiosity_gathered * material->color;
                    //std::cout << radiosity_gathered << std::endl;

                    // Phong specular
                    Vec3 l = dir.normalize();
                    Vec3 r = l - 2.0*l.dot(intersection.normal)*intersection.normal;
                    double vdotr = ray.direction.dot(r);
                    if(vdotr > 0){
                        radiosity_gathered.add(luminaire->get_exitance()*pow(vdotr,20)*material->rho_s);
                    }
                }
            } // sample iterator
            radiosity_gathered /= samples->size();
            local_intensity += radiosity_gathered;
        } // luminaire iterator
    }

    // Add color

    /*
     * REFLECTED
     */
    Color reflected_intensity;

    // Only process if intersected from outside of object
    if(intersection.result == 1){
        // Reflected ray
        Vec3 r = ray.direction - (intersection.normal*(ray.direction.dot(intersection.normal))*2);
        Ray reflected_ray(intersection.position, r, intersection.primitive);

        // Recursively trace
        Trace reflected_trace = trace_ray(reflected_ray, rho * material->rho_r, depth+1);
        if(!reflected_trace.empty){
            reflected_intensity = reflected_trace.color * material->rho_r;
        }
    }

    /*
     * TRANSMITTED
     */

    Color transmitted_intensity;
    Vec3 normal = intersection.result * intersection.normal;
    double cos_t1 = -normal.dot(ray.direction);
    double n1 = 1.0;
    double n2 = material->n;
    if(intersection.result == 1){
    }else{
        cout << "D";
    }
    double cos_t2_sq = 1.0 - pow(n1/n2,2) * pow(1-cos_t1,2);
    if(cos_t2_sq > 0){
        // cos(t)^2
        double cos_t2 = sqrt(cos_t2_sq);

        // Direction for refracted ray
        Vec3 v_refract = (n1/n2)*ray.direction + (n1/n2*cos_t1 - cos_t2)*intersection.normal;

        // Create ray
        Ray transmitted_ray(intersection.position, v_refract, intersection.primitive, (intersection.result == 1)?TRANSMITTED:ORIGINAL);
        //Ray transmitted_ray(intersection.position, v_refract, intersection.primitive);

        Trace transmitted_trace = trace_ray(transmitted_ray, rho * material->rho_t, depth+1);

        if(!transmitted_trace.empty){
            transmitted_intensity = transmitted_trace.color * material->rho_t;
        }
    }




    /*
     * Result
     */
    return Trace(local_intensity+reflected_intensity+transmitted_intensity, TRACE_VALID, intersection.primitive);
    
}


/**
 * Finds the nearest intersection given a point and a direction.
 *
 * @param  p the starting point
 * @param  v the direction
 * @param  exclude an primitive to exclude from the check
 * @return an intersection
 */
#define MIN_T 0.0001
Intersection Camera::find_nearest_intersection(const Ray& ray) const{
    double min_t = DBL_MAX;
    Intersection nearest_intersection;
    for(vector<Primitive*>::const_iterator it = scene->primitives.begin(); it != scene->primitives.end(); ++it){
        Primitive *primitive = *it;

        // We don't want to let an object intersect with itself
        //if(primitive == ray.source && ray.type != TRANSMITTED){
        //    continue;
        //}

        Intersection sect = primitive->intersect(ray);

        // Does not intersect
        if(sect.t <= MIN_T){
            continue;
        }

        // Otherwise there are two intersections
        // Update closest intersection
        if(sect.t < min_t){
            min_t = sect.t;
            nearest_intersection = sect;
        }
    }

    return nearest_intersection;
}
