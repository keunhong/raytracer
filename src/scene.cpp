#include "scene.hpp"

Scene::Scene() : scale(1){}


Scene::Scene(double scale_) : scale(scale_){}

Scene::~Scene(){
    for(vector<Primitive*>::iterator it = primitives.begin(); it != primitives.end(); ++it){
        delete *it;
    }
    for(vector<LightSource*>::iterator it = lightsources.begin(); it != lightsources.end(); ++it){
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

LightSource *Scene::create_lightsource(Vec3 position, Color intensity){
    LightSource *lightsource = new LightSource(position, intensity);
    lightsources.push_back(lightsource);

    Sphere *lightsource_primitive = new Sphere(position, 10.0, NULL);
    lightsource_primitive->set_light(true);
    primitives.push_back(lightsource_primitive);
    
    return lightsource;
}



