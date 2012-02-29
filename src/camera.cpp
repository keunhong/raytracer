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
Camera::Camera(const Scene *scene_, Vec3 image_center, int width_, int height_, double fovx_) : width(width_), height(height_), fovx(fovx_){
    fovy = height/width * fovx;
    focal_length = height / (2*std::tan(fovx/2));

    focal_point.x = image_center.x;
    focal_point.y = image_center.y;
    focal_point.z = image_center.z + focal_length;

    scene = scene_;

    std::cout << "Camera: focal_point=" << focal_point 
              << ", image_center=" << Vec3(focal_point.x, focal_point.y, focal_point.z-focal_length)
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
void Camera::render(cv::Mat &im, int rays_per_pixel) const{
    for(int u = 0; u < width; u++){
        if(u%10==0)
            cout << (double)u/width*100 << "%" << endl;
        for(int v = 0; v < height; v++){
            Color color;
            double x = u - width/2;
            double y = v - height/2;
            for(int i = 0; i < rays_per_pixel; i++){
                x += 1.0/rays_per_pixel;
                y += 1.0/rays_per_pixel;

                Vec3 image_point(focal_point.x+x, focal_point.y+y, focal_point.z-focal_length);

                Vec3 ray_start = image_point;
                Vec3 ray_direction = image_point-focal_point;
                Ray ray(ray_start, ray_direction.normalize(), NULL);
                Trace trace = trace_ray(ray, 1.0, 0);
                color.add(trace.color);
            }
            
            color = color/rays_per_pixel;

            // Image will be inverted
            im.at<cv::Vec3b>(height-1-v,u)[0] = std::min(255.0,color.b * 255);
            im.at<cv::Vec3b>(height-1-v,u)[1] = std::min(255.0,color.g * 255);
            im.at<cv::Vec3b>(height-1-v,u)[2] = std::min(255.0,color.r * 255);
        }
    }
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
        return Trace(Color::BLACK, TRACE_EMPTY, NULL);
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
        for(vector<Luminaire*>::const_iterator it = scene->luminaires.begin(); it != scene->luminaires.end(); ++it){
            Luminaire *luminaire = *it;
            Vec3 dir = luminaire->get_position() - intersection.position;
            
            // Shoot shadow ray
            Ray shadow_ray(intersection.position, dir, intersection.primitive);
            Intersection shadow_intersection = find_nearest_intersection(shadow_ray); 

            if(!(shadow_intersection.result == 1 && !shadow_intersection.primitive->is_luminaire())){
                // Diffuse
                Color intensity = luminaire->get_intensity() * material->rho_d;
                double ndotl = dir.normalize()*intersection.normal;
                ndotl = std::max(0.0, ndotl);
                local_intensity.add(intensity * ndotl);
                local_intensity = local_intensity * material->color;

                // Phone specular
                Vec3 l = dir.normalize();
                Vec3 r = l - 2.0*l.dot(intersection.normal)*intersection.normal;
                double vdotr = ray.direction.dot(r);
                if(vdotr > 0){
                    local_intensity.add(luminaire->get_intensity()*pow(vdotr,20)*material->rho_s);
                }
            }
        }
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
    double cos_t1 = intersection.normal.dot(-ray.direction);
    double n1, n2;
    if(intersection.result == 1){
        n1 = 1.0;
        n2 = material->n;
    }else{
        cout << "Derp" << endl;
        n1 = material->n;
        n2 = 1.0;
    }
    double cos_t2_sq = 1 - pow(n1/n2,2) * pow(1-cos_t1,2);
    if(cos_t2_sq > 0){
        // cos(t)^2
        double cos_t2 = sqrt(cos_t2_sq);

        // Direction for refracted ray
        Vec3 v_refract = (n1/n2)*ray.direction + (n1/n2*cos_t1 - cos_t2)*intersection.normal;

        // Create ray
        Ray transmitted_ray(intersection.position, v_refract, intersection.primitive);

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
Intersection Camera::find_nearest_intersection(const Ray& ray) const{
    double min_t = DBL_MAX;
    Intersection nearest_intersection;
    for(vector<Primitive*>::const_iterator it = scene->primitives.begin(); it != scene->primitives.end(); ++it){
        Primitive *primitive = *it;

        // We don't want to let an object intersect with itself
        if(primitive == ray.source && ray.type != TRANSMITTED){
            continue;
        }

        Intersection sect = primitive->intersect(ray);

        // Does not intersect
        //if(sect.result <= 0){
        if(sect.t <= 0){
            continue;
        }

        // Otherwise there are two intersections
        if(sect.t < min_t){
            min_t = sect.t;
            nearest_intersection = sect;
        }
    }

    return nearest_intersection;
}
