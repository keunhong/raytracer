#include "luminaire.hpp"


Luminaire::Luminaire(){}
Luminaire::Luminaire(Vec3 position_, Color intensity_) : position(position_), intensity(intensity_){}

const Vec3& Luminaire::get_position(){
    return position;
}

const Color& Luminaire::get_intensity(){
    return intensity;
}
