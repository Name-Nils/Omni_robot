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
    static const char Settings_chars[] = { // the enum Settings and this array are connected and work together to define the functioncality
        'A',
        'E',
        'S',
        'T',
        'M',
        'G'
    };
    static const int amount_settings = sizeof(Settings_chars) / sizeof(char);

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


    template<typename gen>
    struct Id
    {
        char id;
        gen data; // can be interpreted as a int or bool later 
        
        Id() = default;
        Id(const Id & i)
        {
            this->data = i.data;
            this->id = i.id;
        }
        Id(char id)
        {
            this->id = id;
        }
        Id(char id, gen data)
        {
            this->id = id;
            this->data = data;
        }
    };

    template<int length, typename gen>
    struct Ids
    {
        Id<gen> identifier[length];

        Ids() = default;
        Ids(const Ids& i)
        {
            for (int a = 0; a < length; a++)
            {
                this->identifier[a] = i.identifier[a];
            }
        }
        Ids(const char ids[length])
        {
            for (int i = 0; i < length; i++)
            {
                identifier[i].id = ids[i];
            }
        }
    };

    class Parsing
    {
        Ids<amount_settings, bool> settings = Ids<amount_settings, bool>(Settings_chars);
        Ids<amount_movement, float> movement = Ids<amount_movement, float>(Movement_chars);


    public:
        Parsing() = default;
        Parsing(const Parsing & p)
        {
            this->movement = p.movement;
            this->settings = p.settings;
        }

        void parse(const char *, int length);
        bool* get_settings();
        double* get_movement(); // can use the enums to go throught the settings witch are loaded into these arrays that are returned
    
        void print_ids()
        {
            for (int i = 0; i < amount_movement; i++)
            {
                Serial.print(Movement_chars[i]);
                Serial.print("  ");
                Serial.println(movement.identifier[i].id);
            }
            Serial.println();
            for (int i = 0; i < amount_settings; i++)
            {
                Serial.print(Settings_chars[i]);
                Serial.print("  ");
                Serial.println(settings.identifier[i].id);
            }
        } 
    };
} // namespace Usb_control
