#ifndef KP_RAY_H
#define KP_RAY_H

#include "vec3.hpp"

enum RayType {REFLECTED, TRANSMITTED, ORIGINAL};

class Primitive;
class Ray{
  public:
    Vec3 origin;
    Vec3 direction;
    Primitive *source;
    RayType type;
    Ray(Vec3 o, Vec3 d, Primitive *s);
    Ray(Vec3 o, Vec3 d, Primitive *s, RayType t);
};

#endif
