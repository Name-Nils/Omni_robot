#pragma once
#include "Vector.h"

namespace Usb_control
{
    enum Modifiers
    {
    };
    enum Mode
    {
        RELATIVE,
        ABSOLUTE,
        SPEED
    };
    enum Identifiers_names
    {
        Mode,
        Modifier,
        rotation,
        angle,
        size,
        x,
        y
    }; // namespace name


    struct Identifier
    {
        double data;
        char id;

        Identifier() = default;
        Identifier(const Identifier & i)
        {
            data = i.data;
            id = i.id;
        }
        Identifier(char id)
        {
            this->id = id;
        }
    };  

    class Parsing
    {
    public:
        const static int amount_identifiers = 5;
        Identifier Identifiers[amount_identifiers];

        Vector speed_wanted;
        double rotation_wanted = 0;

        Parsing() = default;
        Parsing(const Parsing &p);
        bool init();

        bool parse(const char *string);
    };
} // namespace Usb_control
