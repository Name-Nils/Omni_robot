#pragma once

class Vector
{
public:
    double angle = 0; // in radians 
    double size = 0;

    Vector() = default;
    Vector(const Vector&);
    Vector(double, double); // dont have to define the name of the variables in the definition
};  // although defining the names can sometimes help with understanding the functionality

inline Vector::Vector(const Vector& v)
{
    angle = v.angle;
    size = v.size;
}
inline Vector::Vector(double angle, double size)
{
    this->angle = angle;
    this->size = size;
}