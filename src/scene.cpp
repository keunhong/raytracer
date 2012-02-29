#include "scene.hpp"

Scene::Scene() : scale(1){}


Scene::Scene(double scale_) : scale(scale_){}

Scene::~Scene(){
    for(vector<Primitive*>::iterator it = primitives.begin(); it != primitives.end(); ++it){
        delete *it;
    }
    for(vector<Luminaire*>::iterator it = luminaires.begin(); it != luminaires.end(); ++it){
        delete *it;
    }
}

Sphere *Scene::create_sphere(Vec3 position, double radius, Material *material){
    Sphere *sphere = new Sphere(position, radius, material);
    primitives.push_back(sphere);

    return sphere;
}


Plane *Scene::create_plane(Vec3 n, Vec3 r0, Material *material){
    Plane *plane = new Plane(n, r0, material);
    primitives.push_back(plane);

    return plane;
}

Luminaire *Scene::create_luminaire(Vec3 position, Color intensity){
    Luminaire *luminaire = new Luminaire(position, intensity);
    luminaires.push_back(luminaire);

    Sphere *luminaire_primitive = new Sphere(position, 10.0, NULL);
    luminaire_primitive->set_luminaire(true);
    primitives.push_back(luminaire_primitive);
    
    return luminaire;
}



