#ifndef KP_SPHERE_H
#define KP_SPHERE_H

#include <cmath>
#include <cfloat>
#include "vec3.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "ray.hpp"

class Sphere : public Primitive{
  private:
    Vec3 center;
    double radius;
    vector<Vec3> samples;

  public:
    Sphere();
    Sphere(Vec3 center, double radius, Material *material);
    virtual Intersection intersect(const Ray& ray);
    virtual Vec3 get_position() const;
    virtual vector<Vec3> *get_samples();
};


#include "material.hpp"

#endif
