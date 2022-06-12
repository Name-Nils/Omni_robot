#include "usb_control.h"
#include <Arduino.h>

namespace Helper
{
    const char * alphabet = "QWERTYUIOPÅASDFGHJKLÖÄZXCVBNMqwertyuiopåasdfghjklöäzxcvbnm     ";
    const char * numerical = "1234567890.-";

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


namespace Usb
{
    Usb_control::Parsing control;
} // namespace Usb


namespace Usb_control
{
    void Parsing::parse(const char * data, int size)
    { // size is the amount of bytes/chars 
        bool setting;
        bool* setting_ptr;
        double* movement_ptr;
        String current_number;

        for (int d = 0; d < size; d++)
        {
            const char current = data[d];

            if (Helper::regex(Helper::numerical, current))
            {
                current_number += current;
                continue;
            }

            if (Helper::regex(Helper::alphabet, current) || d == size - 1) // the or statememnt to make sure all are evaluated in case of a non a last item being used
            {
                if (current_number != "")
                {
                    if (setting)
                    {
                        *setting_ptr = (bool)round(current_number.toDouble());
                    }
                    else
                    {
                        *movement_ptr = current_number.toDouble();
                    }
                    current_number = "";
                }

                for (int s = 0; s < amount_settings; s++)
                {
                    if (settings.ids[s] == current)
                    {
                        setting_ptr = &settings.data[s];
                        setting = true;
                    }
                }
                for (int m = 0; m < amount_movement; m++)
                {
                    if (movement.ids[m] == current)
                    {
                        movement_ptr = &movement.data[m];
                        setting = false;
                    }
                }
            }
        }
    }

    bool* Parsing::get_settings()
    {
        return settings.data;
    }
    double* Parsing::get_movement()
    {
        return movement.data;
    }
} // namespace Usb_control
