#ifndef KP_MATERIAL_H
#define KP_MATERIAL_H

#include <iostream>
#include "color.hpp"

class Material{
    friend class Camera;

  private:
    Color color;
    double rho_d; // diffuse albedo
    double rho_s; // specular albedo
    double rho_r; // specular albedo
    double rho_t; // translucent albedo
    double n; // refractive index
  public:
    Material();
    Material(const Color& c, double rho_d_, double rho_s_, double rho_r_, double rho_t_, double n_);
    Color get_color() const;

};

#endif
