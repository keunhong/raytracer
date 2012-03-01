#ifndef KP_PLANE_H
#define KP_PLANE_H

#include <cmath>
#include <cfloat>
#include "vec3.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "ray.hpp"
#include <iostream>

using std::cout;
using std::endl;


class Plane : public Primitive{
  private:
    Vec3 normal;
    Vec3 r0;
    vector<Vec3> samples;

  public:
    Plane();
    Plane(Vec3 n, Vec3 r0_, Material *m);
    virtual Intersection intersect(const Ray& ray);
    virtual Vec3 get_position() const;
    virtual vector<Vec3> *get_samples();
};

#endif
