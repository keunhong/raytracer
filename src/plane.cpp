#include "plane.hpp"

Plane::Plane(){}
Plane::Plane(Vec3 n, Vec3 r0_, Material *m) : Primitive(){
    this->normal = n;
    this->r0 = r0_;
    this->material = m;
    this->samples = vector<Vec3>();
    this->samples.push_back(r0_);
};

Vec3 Plane::get_position() const{
    return this->r0;
}

// TODO: actually implement samples
vector<Vec3> *Plane::get_samples(){
    return &this->samples;
}

Intersection Plane::intersect(const Ray& ray){
    Vec3 p = ray.origin;
    Vec3 v = ray.direction;
    double vdotn = v*normal;

    // If ray is parallel it will never intersect
    if(vdotn <= DBL_EPSILON && vdotn >= DBL_EPSILON){
        return Intersection(this, DBL_MAX, normal, Vec3(), 0);
    }

    double t = ((r0-p)*normal)/(vdotn);
    Vec3 position = p + v*t;

    // A ray should only ever go forwards
    if(t <= DBL_EPSILON){
        return Intersection(this, t, normal, position, 0);
    }

    return Intersection(this, t, normal, position, 1);
}
