#include "scene.hpp"

Scene::Scene() : scale(1){}


Scene::Scene(double scale_) : scale(scale_){}

Scene::~Scene(){
    for(vector<Primitive*>::iterator it = primitives.begin(); it != primitives.end(); ++it){
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

Primitive *Scene::create_luminaire(Primitive *primitive, Color intensity){
    //Luminaire *luminaire = new Luminaire(position, intensity);

    primitive->set_exitance(intensity);

    luminaires.push_back(primitive);
    
    return primitive;
}

Rectangle *Scene::create_rectangle(Vec3 p0, Vec3 a, Vec3 b, Material *material){
    Rectangle *rectangle = new Rectangle(p0, a, b, material);
    primitives.push_back(rectangle);

    return rectangle;
}

