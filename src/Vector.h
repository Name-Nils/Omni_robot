#pragma once

class Vector
{
public:
    double angle = 0; // in radians 
    double size = 0;
    double x = 0;
    double y  =0;

    Vector() = default;
    Vector(const Vector&);
    Vector(double, double); // dont have to define the name of the variables in the definition
    Vector(double, double, bool cartesian);
    void print(bool);
};  // although defining the names can sometimes help with understanding the functionality

inline Vector::Vector(const Vector& v)
{
    angle = v.angle;
    size = v.size;
    x = v.x;
    y = v.x;
}
inline Vector::Vector(double angle, double size)
{
    this->angle = angle;
    this->size = size;
}
inline Vector::Vector(double x, double y, bool cartesian)
{
    if (!cartesian)
    {
        Vector(x, y);
        return;
    }
    this->x = x;
    this->y = y;
    this->angle = atan2(y, x);
    this->size = sqrt(pow(x, 2) + pow(y, 2));
}

inline void Vector::print(bool cartesian)
{
    if (cartesian)
    {
        Serial.println("X" + String(x) + " Y" + String(y));
    }
}