#include "primitive.hpp"

int Primitive::counter = 0;
Primitive::Primitive(){
    key = ++counter;
    m_exitance = Color::BLACK;
}

Primitive::Primitive(Material *m){
    key = ++counter;
    m_exitance = Color::BLACK;
    material = m;
}

const Material *Primitive::get_material() const{
    return material;
}

int Primitive::get_key() const{
    return key;
}

void Primitive::set_exitance(Color e){
    m_exitance = e;
}

Color Primitive::get_exitance() const{
    return m_exitance;
}

bool Primitive::is_luminaire() const{
    //return m_is_luminaire;
    return m_exitance != Color::BLACK;
}

void Primitive::tick(double time_step){

}

void Primitive::set_velocity(Vec3 v){
    velocity = v;
}
