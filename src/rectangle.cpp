#include "rectangle.hpp"

// TODO: make better definition
Rectangle::Rectangle(){}
Rectangle::Rectangle(Vec3 n, Vec3 c, double w, double h, Material *m) : Primitive(m){
    this->normal = n;
    this->center = c;
    this->width = w;
    this->height = h;
};

#define SAMPLES_X 10
#define SAMPLES_Y 10
void Rectangle::generate_samples(){
    /*double step_x = width/SAMPLES_X;
    double step_y = height/SAMPLES_Y;
    Vec3 top_right = 
    for(int i = 0; i < SAMPLES_X; i++){
        for(int j = 0; j < SAMPLES_Y; j++){
            
        }
    }*/
}

Vec3 Rectangle::get_position() const{
    return center;
}

vector<Vec3>& Rectangle::get_samples(){
    return this->samples;
}

Intersection Rectangle::intersect(const Ray& ray){
    Vec3 p = ray.origin;
    Vec3 v = ray.direction;
    double vdotn = v*normal;

    // If ray is parallel it will never intersect
    if(vdotn <= DBL_EPSILON && vdotn >= DBL_EPSILON){
        return Intersection(this, DBL_MAX, normal, Vec3(), 0);
    }

    double t = ((center-p)*normal)/(vdotn);
    Vec3 position = p + v*t;

    // A ray should only ever go forwards
    if(t <= DBL_EPSILON){
        return Intersection(this, t, normal, position, 0);
    }

    // Restrict width and height
    Vec3 height_vec = Vec3(0, 0, 1);
    Vec3 width_vec = Vec3(1, 0, 0);
    Vec3 diff = position-center;
    double height_t = std::abs(height_vec.dot(diff));
    double width_t = std::abs(width_vec.dot(diff));

    if(height_t < height && width_t < width){
        return Intersection(this, t, normal, position, 1);
    }else{
        return Intersection(this, 0, normal, position, 0);
    }
}

