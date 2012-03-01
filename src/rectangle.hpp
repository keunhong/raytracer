#ifndef KP_RECTANGLE_H
#define KP_RECTANGLE_H

#include <cmath>
#include <cfloat>
#include "vec3.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "ray.hpp"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;


class Rectangle : public Primitive{
  private:
    Vec3 a;
    Vec3 b;
    Vec3 p0;
    Vec3 p1;
    Vec3 p2;
    Vec3 p3;
    Vec3 normal;
    Vec3 center;
    double width;
    double height;
    vector<Vec3> samples;

  public:
    Rectangle();
    Rectangle(Vec3 p0, Vec3 a, Vec3 b, Material *m);
    virtual Intersection intersect(const Ray& ray);
    virtual Vec3 get_position() const;
    virtual vector<Vec3> *get_samples();
    void generate_samples(int samples_a, int samples_b);
};

#endif
