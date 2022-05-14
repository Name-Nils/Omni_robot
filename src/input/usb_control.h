#pragma once
#include "Vector.h"

namespace Usb_control
{
    class Parsing
    {
    public:
        Vector speed_wanted;
        double rotation_wanted = 0;
        

        Parsing() = default;
        Parsing(const Parsing & p);

        void parse(const char* string);
    };
} // namespace Usb_control
