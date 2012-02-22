#include "lightsource.hpp"


LightSource::LightSource(){}
LightSource::LightSource(Vec3 position_, Color intensity_) : position(position_), intensity(intensity_){}

const Vec3& LightSource::get_position(){
    return position;
}

const Color& LightSource::get_intensity(){
    return intensity;
}
