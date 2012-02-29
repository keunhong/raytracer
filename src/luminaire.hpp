#ifndef KP_LIGHTSOURCE_H
#define KP_LIGHTSOURCE_H

#include "vec3.hpp"
#include "color.hpp"

class Luminaire{
  private:
    Vec3 position;
    Color intensity;
  public:
    Luminaire();
    Luminaire(Vec3 position_, Color intensity_);
    const Vec3& get_position();
    const Color& get_intensity();
};

#endif
