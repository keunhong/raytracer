#ifndef KP_SCENE_H
#define KP_SCENE_H

#include <vector>
#include <cfloat>
#include "sphere.hpp"
#include "plane.hpp"
#include "luminaire.hpp"
#include "color.hpp"
#include "vec3.hpp"

using std::vector;

class Scene{
    friend class Camera;

  private:
    vector<Primitive*> primitives;
    vector<Luminaire*> luminaires;
    double scale;

  public:
    Scene();
    Scene(double scale);
    ~Scene();
    Sphere *create_sphere(Vec3 position, double radius, Material *material);
    Plane *create_plane(Vec3 n, Vec3 r0, Material *material);
    Luminaire *create_luminaire(Vec3 position, Color intensity);
};

#endif
