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
    char* string(int, bool, int);
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

inline char* Position::string(int type, bool rotation, int digits)
{
    char * rtn = "pos ";

    if (type == Coordinate::CARTESIAN)
    {
        strcat(rtn, "X");
        strcat(rtn, String(x,digits).c_str());
        strcat(rtn, " Y");
        strcat(rtn, String(y,digits).c_str());
    }
    else if (type == Coordinate::POLAR)
    {
        strcat(rtn, "A");
        strcat(rtn, String(a,digits).c_str());
        strcat(rtn, " S");
        strcat(rtn, String(s,digits).c_str());
    }

    if (rotation)
    {
        strcat(rtn, " R");
        strcat(rtn, String(r,digits).c_str());
    }
}

inline void Position::reset()
{
    x = y = r = s = a = 0;
}