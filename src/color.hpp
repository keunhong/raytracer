#ifndef KP_COLOR_H
#define KP_COLOR_H

#include <iostream>

class Color{
  public:
    double r, g, b, a;
    Color();
    Color(double r_, double g_, double b_);
    Color(double r_, double g_, double b_, double a_);
    Color& add(const Color& c);
    
    Color operator+(const Color& right) const;
    Color operator*(const Color& right) const;
    Color operator*(double s) const;
    Color operator/(double s) const;

    friend std::ostream& operator<<(std::ostream& os, const Color& c);


    // Static Constants
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color WHITE;
    static const Color BLACK;
    static const Color CYAN;
    static const Color MAGENTA;
    static const Color YELLOW;
};

#endif
