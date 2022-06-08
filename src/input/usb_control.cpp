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
    void Parsing::parse(const char * data)
    {
        
    }
} // namespace Usb_control
