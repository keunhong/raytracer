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
    Vec3 normal;
    Vec3 center;
    double width;
    double height;
    vector<Vec3> samples;

    void generate_samples();

  public:
    Rectangle();
    Rectangle(Vec3 n, Vec3 c, double w, double h, Material *m);
    virtual Intersection intersect(const Ray& ray);
    virtual Vec3 get_position() const;
    vector<Vec3>& get_samples();
};

#endif
