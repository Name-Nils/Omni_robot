#include "usb_control.h"
#include <Arduino.h>

namespace Helper
{
    bool regex(const char * is_in, char check)
    {
        int i = 0;
        while (is_in[i] != '\0')
        {
            if (is_in[i] == check) return true;
            i++;
        }
        return false;
    }

    double to_double(const char * a)
    {
        return String(a).toFloat();
    }
} // namespace Helper



namespace Usb_control
{
    Parsing::Parsing(const Parsing & p)
    {
        speed_wanted = p.speed_wanted;
        rotation_wanted = p.rotation_wanted;

        for (int i = 0; i < p.amount_identifiers; i++)
        {
            Identifiers[i] = p.Identifiers[i];
        }
    }
    bool Parsing::init()
    {
        Identifiers[Identifiers_names::Mode] = Identifier('G');
        Identifiers[Identifiers_names::Modifier] = Identifier('H');
        Identifiers[Identifiers_names::rotation] = Identifier('R');
        Identifiers[Identifiers_names::size] = Identifier('S');
        Identifiers[Identifiers_names::angle] = Identifier('A');

        return true;
    }

    void Parsing::parse(const char * string)
    {
        static const char * numbers = "1234567890-.";

        int length = 0;
        while (string[length] != '\0') length ++;

        Identifier *current_id;
        String current_number  = "";

        for (int i = 0; i < length + 1; i++)
        {
            if (i != length && Helper::regex(numbers, string[i]))
            {
                current_number += string[i];
                continue;            
            }


            for (int id = 0; id < amount_identifiers; id++)
            {
                if (string[i] == Identifiers[id].id || i == length)
                {
                    if (current_number != "")
                    {
                        current_id->data = current_number.toDouble();
                        current_number = "";
                    }

                    current_id = &Identifiers[id];
                    break;
                }
            }
        }
        speed_wanted = Vector(Identifiers[Identifiers_names::angle].data, Identifiers[Identifiers_names::size].data);
        rotation_wanted = Identifiers[Identifiers_names::rotation].data;
    }
} // namespace Usb_control
