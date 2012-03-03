
#include "ray.hpp"

Ray::Ray(Vec3 o, Vec3 d, Primitive *s) : origin(o), direction(d), source(s){
    type = ORIGINAL;
}

Ray::Ray(Vec3 o, Vec3 d, Primitive *s, RayType t) : origin(o), direction(d), source(s), type(t){
}
