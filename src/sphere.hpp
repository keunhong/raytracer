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

  public:
    Sphere();
    Sphere(Vec3 center, double radius, Material *material);
    virtual Intersection intersect(const Ray& ray);
};


#include "material.hpp"

#endif
