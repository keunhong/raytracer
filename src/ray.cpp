
#include "ray.hpp"

Ray::Ray(Vec3 o, Vec3 d, Primitive *s) : origin(o), direction(d), source(s){
    direction.normalize();
}
