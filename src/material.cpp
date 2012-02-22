#include "material.hpp"

Material::Material(){
    std::cout << "Default constructor for material called. This will do NOTHING." << std::endl;
}
Material::Material(const Color& c, double rho_d_, double rho_s_, double rho_r_, double rho_t_, double n_): color(c), rho_d(rho_d_), rho_s(rho_s_), rho_r(rho_r_), rho_t(rho_t_), n(n_){}

Color Material::get_color() const{
    return color;
}
