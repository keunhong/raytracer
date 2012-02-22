#ifndef KP_PRIMITIVE_H
#define KP_PRIMITIVE_H

#include "material.hpp"
#include "vec3.hpp"
#include "ray.hpp"

struct Intersection{
    Primitive *primitive;
    int result;
    double t;
    Vec3 normal;
    Vec3 position;
    Intersection() : primitive(NULL), result(0){}
    Intersection(Primitive *e, double t_, Vec3 n, Vec3 p, int r){
        primitive = e;
        result = r;
        t = t_;
        normal = n;
        position = p;
    }
};

class Primitive{
    static int counter;
    int key;

  protected:
    const Material *material;
    bool m_is_light;

  public:
    Primitive();
    virtual Intersection intersect(const Ray& ray) = 0;
    virtual const Material *get_material() const;
    int get_key() const;
    void set_light(bool v);
    bool is_light() const;
};

#endif
