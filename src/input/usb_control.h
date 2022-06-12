#pragma once
#include <Arduino.h>

namespace Usb_control
{
    enum Settings
    {
        ABSOLUTE,
        RELATIVE,
        VELOCITY,
        RESET_POS,
        MOTOR,
        GETPOS
    };
    const char Settings_chars[] = { // the enum Settings and this array are connected and work together to define the functioncality
        'A',
        'E',
        'S',
        'T',
        'M',
        'G'
    };
    const int amount_settings = sizeof(Settings_chars) / sizeof(char);

    enum Movement
    {
        ROTATION, // for simpler calculations the movement system has been switched to x and y coordinates instead of polar form, although this is easy to use and switch between
        X,
        Y,
        SPEED,
        ACCEL,
        Z
    };
    static const char Movement_chars[] = {
        'R',
        'X',
        'Y',
        'V',
        'P',
        'Z'
    };
    static const int amount_movement = sizeof(Movement_chars) / sizeof(char);

    template<int len, typename gen>
    struct Data
    {
        const char *ids; // i have removed the extra struct and am instead holding the data in an array inside this class
        gen data[len];

        Data() = default;
        Data(const Data &d)
        {
            ids = d.ids;
            for (int i = 0; i < len; i++)
            {
                data[i] = d.data[i];
            }
        }
        Data(const char * id)
        {
            ids = id;
            for (int i = 0; i < len; i++)
            {
                data[i] = 0;
            }
        }
    };

    class Parsing
    {
        Data<amount_settings, bool> settings = Data<amount_settings, bool>(Settings_chars);
        Data<amount_movement, double> movement = Data<amount_movement, double>(Movement_chars);
    
    public:
        void parse(const char *, int);
        double* get_movement();
        bool* get_settings();
    };
} // namespace Usb_control
