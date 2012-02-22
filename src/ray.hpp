#ifndef KP_RAY_H
#define KP_RAY_H

#include "vec3.hpp"

class Primitive;
class Ray{
  public:
    Vec3 origin;
    Vec3 direction;
    Primitive *source;
    Ray(Vec3 o, Vec3 d, Primitive *s);
};

#endif
