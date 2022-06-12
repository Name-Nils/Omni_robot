#include <Arduino.h>


namespace Coordinate
{
    enum Type
    {
        CARTESIAN,
        POLAR
    };
} // namespace Coordinate



class Position
{
public:
    double x = 0; // cartesian coordinates
    double y = 0;

    double r = 0; // rotation is not really accounted for since it has to be clearly defined to use 

    double s = 0; // polar coordinates
    double a = 0;

private:
    void calculate(int);

public:
    Position() = default;
    Position(double, double, double, int);
    Position(double, double, int);
    String string(int, bool, int);
    void reset();
};

inline void Position::calculate(int type)
{
    if (type == Coordinate::CARTESIAN)
    {
        x = cos(a) * s;
        y = sin(a) * s;
    }
    else if (type == Coordinate::POLAR)
    {
        a = atan2(y, x);
        s = sqrt(pow(x, 2) + pow(y, 2));
    }
}

inline Position::Position(double x, double y, double r, int type)
{
    if (type == Coordinate::CARTESIAN)
    {
        this->x = x;
        this->y = y;
        this->r = r;

        calculate(Coordinate::POLAR);
    }
    else if (type == Coordinate::POLAR)
    {
        this->a = x;
        this->s = y;
        this->r = r;

        calculate(Coordinate::CARTESIAN);
    }
}

inline Position::Position(double x, double y, int type)
{
    Position(x, y, 0.0, type);
}

inline String Position::string(int type, bool rotation, int digits)
{
    String rtn = "pos ";

    if (type == Coordinate::CARTESIAN)
    {
        rtn += String("X");
        rtn += String(x,digits).c_str();
        rtn += String(" Y");
        rtn += String(y,digits).c_str();
    }
    else if (type == Coordinate::POLAR)
    {
        rtn += String("A");
        rtn += String(a,digits).c_str();
        rtn += String(" S");
        rtn += String(s,digits).c_str();
    }

    if (rotation)
    {
        rtn += String(" R");
        rtn += String(r,digits).c_str();
    }

    return rtn;
}

inline void Position::reset()
{
    x = y = r = s = a = 0;
}