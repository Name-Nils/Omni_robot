#pragma once
#include "Vector.h"

namespace Usb_control
{
    enum Settings
    {
        ABSOLUTE,
        RELATIVE,
        SPEED
    };
    const char * Settings_chars[] = { // the enum Settings and this array are connected and work together to define the functioncality
        "A",
        "E",
        "S"
    };
    const int amount_settings = sizeof(Settings_chars) / sizeof(char);

    enum Movement
    {
        ROTATION, // for simpler calculations the movement system has been switched to x and y coordinates instead of polar form, although this is easy to use and switch between
        X,
        Y
    };
    const char * Movement_chars[] = {
        "R",
        "X",
        "Y"
    };
    const int amount_movement = sizeof(Movement_chars) / sizeof(char);


    template<typename gen>
    struct Id
    {
        char * id;
        gen data; // can be interpreted as a int or bool later 
        
        Id() = default;
        Id(const Id & i)
        {
            this->data = i.data;
            this->id = i.id;
        }
        Id(char * id)
        {
            this->id = id;
        }
        Id(char * id, gen data)
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
            this->identifier = i.identifier;
        }
        Ids(const char * ids[length])
        {
            for (int i = 0; i < length; i++)
            {
                this->identifier[i] = Id<gen>(ids[i]); // should set the ids to the correct value and in the correct correspinding enum number
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

        void parse(const char *);
    };
} // namespace Usb_control
