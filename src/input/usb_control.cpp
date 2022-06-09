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
    void Parsing::parse(const char * data)
    {
        int length = 0;
        while (data[length] != '\0') length ++;

        char * current_number = "";
        bool setting;
        float * movement_ptr;
        bool * settings_ptr;
        for (int i = 0; i < length + 1; i++)
        {
            if (Helper::regex(Helper::numerical, data[i]))
            {
                current_number += data[i];
                continue;
            }

            if (Helper::regex(Helper::alphabet, data[i]) || length == i)
            {
                if (current_number != "")
                {
                    if (setting)
                    {
                        double n = Helper::to_double(current_number);
                        bool save = false;
                        if (n > 0.5) save = true;

                        *settings_ptr = save;
                    }
                    else
                    {
                        *movement_ptr = Helper::to_double(current_number);
                    }

                    current_number = "";
                }


                for (int s = 0; s < amount_settings; s++)
                {
                    if (Helper::regex(settings.identifier[s].id, data[i]))
                    {
                        setting = true;
                        settings_ptr = &settings.identifier[s].data;
                        break;
                    }
                }
            
                for (int m = 0; m < amount_movement; m++)
                {
                    if (Helper::regex(movement.identifier[m].id, data[i]))
                    {
                        setting = false;
                        movement_ptr = &movement.identifier[m].data;
                        break;
                    }
                }
            }
        }
    }

    bool* Parsing::get_settings()
    {
        bool s[amount_settings];
        for (int i = 0; i < amount_settings; i++)
        {
            s[i] = settings.identifier[i].data;
        }
        return s;
    }
    double* Parsing::get_movement()
    {
        double m[amount_movement];
        for (int i = 0; i < amount_movement; i++)
        {
            m[i] = movement.identifier[i].data;
        }
        return m;
    }
} // namespace Usb_control
