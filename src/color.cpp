#include "color.hpp"

const Color Color::RED(1.0, 0.0, 0.0, 1.0);
const Color Color::GREEN(0.0, 1.0, 0.0, 1.0);
const Color Color::BLUE(0.0, 0.0, 1.0, 1.0);
const Color Color::WHITE(1.0, 1.0, 1.0, 1.0);
const Color Color::BLACK(0.0, 0.0, 0.0, 1.0);
const Color Color::CYAN(0.0, 1.0, 1.0, 1.0);
const Color Color::MAGENTA(1.0, 0.0, 1.0, 1.0);
const Color Color::YELLOW(1.0, 1.0, 0.0, 1.0);

Color::Color(){
    r = g = b = 0.0;
    a = 1.0;
}

Color::Color(double r_, double g_, double b_) : r(r_), g(g_), b(b_), a(1.0){}
Color::Color(double r_, double g_, double b_, double a_) : r(r_), g(g_), b(b_), a(a_){}

Color& Color::add(const Color& c){
    r += c.r;
    g += c.g;
    b += c.b;

    return *this;
}






// Vector operators
//
Color Color::operator+(const Color& right) const{
    return Color(r+right.r, g+right.g, b+right.b, 1.0);
}

Color Color::operator*(const Color& right) const{
    return Color(r*right.r, g*right.g, b*right.b, 1.0);
}
Color& Color::operator+=(const Color& c){
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}
Color& Color::operator-=(const Color& c){
    r /= c.r;
    g /= c.g;
    b /= c.b;
    return *this;
}
Color& Color::operator*=(const Color& c){
    r /= c.r;
    g /= c.g;
    b /= c.b;
    return *this;
}
Color& Color::operator/=(const Color& c){
    r /= c.r;
    g /= c.g;
    b /= c.b;
    return *this;
}

// Scaloar operators
//
Color Color::operator*(double s) const{
    return Color(s*r, s*g, s*b, 1.0);
}

Color Color::operator/(double s) const{
    return Color(r/s, g/s, b/s, 1.0);
}
Color& Color::operator-=(double s){
    r -= s;
    g -=s;
    b -=s;
    return *this;
}

Color& Color::operator+=(double s){
    r += s;
    g +=s;
    b +=s;
    return *this;
}

Color& Color::operator*=(double s){
    r *= s;
    g *=s;
    b *=s;
    return *this;
}

Color& Color::operator/=(double s){
    r /= s;
    g /=s;
    b /=s;
    return *this;
}

bool Color::operator==(const Color& c) const{
    return c.r == r && c.b == b && c.g == g;
}


bool Color::operator!=(const Color& c) const{
    return c.r != r || c.b != b || c.g != g;
}


std::ostream& operator<<(std::ostream& os, const Color& c){
    os << "rgb(" <<  c.r << ", " << c.g <<", " << c.b << ")";
    return os;
}
