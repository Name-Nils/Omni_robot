#include <Arduino.h>
#include "motor_control.h"

namespace Motors
{
    motor_control::Motor m1(2, 3, 0, 0), m2(4, 5, 0, 0), m3(6, 7, 0, 0);
} // namespace Motors

namespace Encoder_interrrupts
{
    void m1() // had to have copys of the same function since the interuppts do not take arguments, not even a template argument function worked when i tried it although that might have been my fault
    {
        bool current_a = digitalRead(Motors::m1.encoder_pin_a);
        static bool last_a = current_a;

        if ((last_a == false) && (current_a == true))
        {
            if (digitalRead(Motors::m1.encoder_pin_b) == false)
            {
                Motors::m1.encoder_pos--;
            }
            else
            {
                Motors::m1.encoder_pos++;
            }
        }
        last_a = current_a;
    }
    void m2()
    {
        bool current_a = digitalRead(Motors::m2.encoder_pin_a);
        static bool last_a = current_a;

        if ((last_a == false) && (current_a == true))
        {
            if (digitalRead(Motors::m2.encoder_pin_b) == false)
            {
                Motors::m2.encoder_pos--;
            }
            else
            {
                Motors::m2.encoder_pos++;
            }
        }
        last_a = current_a;
    }
    void m3()
    {
        bool current_a = digitalRead(Motors::m3.encoder_pin_a);
        static bool last_a = current_a;

        if ((last_a == false) && (current_a == true))
        {
            if (digitalRead(Motors::m3.encoder_pin_b) == false)
            {
                Motors::m3.encoder_pos--;
            }
            else
            {
                Motors::m3.encoder_pos++;
            }
        }
        last_a = current_a;
    }

    void init()
    {
        attachInterrupt(digitalPinToInterrupt(Motors::m1.encoder_pin_a), m1, CHANGE);
        attachInterrupt(digitalPinToInterrupt(Motors::m2.encoder_pin_a), m2, CHANGE);
        attachInterrupt(digitalPinToInterrupt(Motors::m3.encoder_pin_a), m3, CHANGE);
    }
} // namespace Encoder_interrrupts