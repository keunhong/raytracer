#include "primitive.hpp"

int Primitive::counter = 0;
Primitive::Primitive(){
    key = ++counter;
    m_is_luminaire = false;
}

const Material *Primitive::get_material() const{
    return material;
}

int Primitive::get_key() const{
    return key;
}


void Primitive::set_luminaire(bool v){
    m_is_luminaire = v;
}

bool Primitive::is_luminaire() const{
    return m_is_luminaire;
}
