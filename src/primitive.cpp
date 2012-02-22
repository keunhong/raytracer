#include "primitive.hpp"

int Primitive::counter = 0;
Primitive::Primitive(){
    key = ++counter;
    m_is_light = false;
}

const Material *Primitive::get_material() const{
    return material;
}

int Primitive::get_key() const{
    return key;
}


void Primitive::set_light(bool v){
    m_is_light = v;
}

bool Primitive::is_light() const{
    return m_is_light;
}
