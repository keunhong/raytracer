#include "rectangle.hpp"

// TODO: make better definition
Rectangle::Rectangle(){}
Rectangle::Rectangle(Vec3 p0, Vec3 a, Vec3 b, Material *m) : Primitive(m){
    this->normal = a.cross(b).normalize();
    this->a = a;
    this->b = b;
    this->p0 = p0;
    this->p1 = p0+a;
    this->p2 = p0 + a + b;
    this->p3 = p0+b;
    this->center = p0 + (0.5*a) + (0.5*b);
    std::cout << "Rectangle " << get_key() << ":" << this->normal << " " << this->center <<  " " << p0 << " " << p1 << " " << p2  << " " << p3 << std::endl;

    generate_samples(1,1);
};

void Rectangle::generate_samples(int samples_a, int samples_b){
    samples.clear();

    if(samples_a <= 1 && samples_b <= 1){
        //samples.push_back(center);
    }

    Vec3 da = a/samples_a;
    Vec3 db = b/samples_b;
    for(int i = 0; i < samples_a; i++){
        for(int j = 0; j < samples_b; j++){
            Vec3 p = p0 + da*i + db*j;

            double randq = ((double)rand()/(double)RAND_MAX);
            double rands = ((double)rand()/(double)RAND_MAX);

            Vec3 sample_point = p+randq*da + rands*db;
            samples.push_back(sample_point);
            //std::cout << sample_point << std::endl;
    //        std::cout << p << " " << sample_point << std::endl;
        }
    }
}

Vec3 Rectangle::get_position() const{
    return center;
}

vector<Vec3> *Rectangle::get_samples(){
    return &this->samples;
}

Intersection Rectangle::intersect(const Ray& ray){
    Vec3 p = ray.origin;
    Vec3 v = ray.direction;
    double vdotn = v*normal;

    // If ray is parallel it will never intersect
    if(vdotn <= DBL_EPSILON && vdotn >= DBL_EPSILON){
        return Intersection(this, DBL_MAX, normal, Vec3(), 0);
    }

    double t = ((center-p)*normal)/(vdotn);
    Vec3 position = p + v*t;

    // A ray should only ever go forwards
    if(t <= DBL_EPSILON){
        return Intersection(this, t, normal, position, 0);
    }

    // Restrict width and height
    bool c0 = (p1-p0).cross(position-p0).dot(normal) >= 0;
    bool c1 = (p2-p1).cross(position-p1).dot(normal) >= 0;
    bool c2 = (p3-p2).cross(position-p2).dot(normal) >= 0;
    bool c3 = (p0-p3).cross(position-p3).dot(normal) >= 0;

    if(c0 && c1 && c2 && c3){
        return Intersection(this, t, normal, position, 1);
    }else{
        return Intersection(this, 0, normal, position, 0);
    }
}

