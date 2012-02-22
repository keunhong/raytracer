#ifndef KP_LIGHTSOURCE_H
#define KP_LIGHTSOURCE_H

#include "vec3.hpp"
#include "color.hpp"

class LightSource{
  private:
    Vec3 position;
    Color intensity;
  public:
    LightSource();
    LightSource(Vec3 position_, Color intensity_);
    const Vec3& get_position();
    const Color& get_intensity();
};

#endif
