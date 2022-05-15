#pragma once

class Vector
{
public:
    double angle = 0; // in radians 
    double size = 0;

    Vector() = default;
    Vector(const Vector& v);
    Vector(double angle, double size);
};

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