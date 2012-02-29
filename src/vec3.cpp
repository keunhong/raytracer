#include "vec3.hpp"

Vec3::Vec3() : x(0), y(0), z(0){}
Vec3::Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_){}

Vec3& Vec3::normalize(){
    double length = sqrt(x*x+y*y+z*z);
    x /= length;
    y /= length;
    z /= length;

    return *this;
}

double Vec3::length() const{
    return sqrt(x*x+y*y+z*z);
}

double Vec3::dot(const Vec3& r) const{
    return x*r.x + y*r.y + z*r.z;
}

double Vec3::operator*(const Vec3& r) const{
    return dot(r);
}

// Scalar operators
Vec3 Vec3::operator*(double s) const{
    return Vec3(s*x, s*y, s*z);
}
Vec3 operator*(double s, const Vec3& v){
    return Vec3(s*v.x, s*v.y, s*v.z);
}

Vec3 Vec3::operator+(double s) const{
    return Vec3(s+x, s+y, s+z);
}
Vec3 operator+(double s, const Vec3& v){
    return Vec3(s+v.x, s+v.y, s+v.z);
}

Vec3 Vec3::operator-(double s) const{
    return Vec3(x-s, y-s, z-s);
}




// Vector operators
Vec3 Vec3::operator+(const Vec3& r) const{
    return Vec3(x+r.x, y+r.y, z+r.z);
}

Vec3 Vec3::operator-(const Vec3& r) const{
    return Vec3(x-r.x, y-r.y, z-r.z);
}

Vec3 Vec3::operator-() const{
    return Vec3(-x, -y, -z);
}


std::ostream& operator<<(std::ostream& os, const Vec3& v){
    os << "(" <<  v.x << ", " << v.y <<", " << v.z << ")";
    return os;
}



