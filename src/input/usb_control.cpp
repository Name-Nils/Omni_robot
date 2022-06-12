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
    void Parsing::parse(const char * data, int length)
    {     
        print_ids();

        String current_number = "";
        bool setting;
        float * movement_ptr;
        bool * settings_ptr;
        for (int i = 0; i < length + 1; i++)
        {
            if (Helper::regex(Helper::numerical, data[i]))
            {
                current_number += data[i];
                Serial.println(current_number);
                continue;
            }

            if (Helper::regex(Helper::alphabet, data[i]) || length == i)
            {
                if (current_number != "")
                {
                    if (setting)
                    {
                        double n = current_number.toDouble();
                        bool save = false;
                        if (n > 0.5) save = true;

                        *settings_ptr = save;
                    }
                    else
                    {
                        *movement_ptr = current_number.toDouble();
                    }

                    current_number = "";
                }


                for (int s = 0; s < amount_settings; s++)
                {
                    const char* id = settings.identifier[s].id;
                    const char d = data[i];

                    Serial.println(String(id) + " == " + String(d));


                    if ((const char *)settings.identifier[s].id == data[i] && data[i] != '\0')
                    {
                        Serial.println("inside !!");
                        setting = true;
                        settings_ptr = &settings.identifier[s].data;
                        break;
                    }
                }
                
            
                
                for (int m = 0; m < amount_movement; m++)
                {
                    break;
                    Serial.println(String(movement.identifier[m].id) + " == " + String(data[i]));
                    if ((const char *)movement.identifier[m].id == data[i] && data[i] != '\0')
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
