#include <Arduino.h>
#include "motor_control.h"

namespace Motors
{
    motor_control::Motor m1(A2, A1, 6, 2, 4), m2(13, A0, 5, 9, 10), m3(11, 12, 3, 7, 8);
} // namespace Motors

namespace Encoder_interrrupts
{
    void m1() // had to have copys of the same function since the interuppts do not take arguments, not even a template argument function worked when i tried it although that might have been my fault
    {
        if (digitalRead(Motors::m1.encoder_pin_b) == false)
        {
            Motors::m1.encoder_position--;
        }
        else
        {
            Motors::m1.encoder_position++;
        }
    }
    void m2()
    {
        if (digitalRead(Motors::m2.encoder_pin_b) == false)
        {
            Motors::m2.encoder_position--;
        }
        else
        {
            Motors::m2.encoder_position++;
        }
    }
    void m3()
    {
        if (digitalRead(Motors::m3.encoder_pin_b) == false)
        {
            Motors::m3.encoder_position--;
        }
        else
        {
            Motors::m3.encoder_position++;
        }
    }

    void init()
    {
        attachInterrupt(digitalPinToInterrupt(Motors::m1.encoder_pin_a), m1, RISING);
        attachInterrupt(digitalPinToInterrupt(Motors::m2.encoder_pin_a), m2, RISING);
        attachInterrupt(digitalPinToInterrupt(Motors::m3.encoder_pin_a), m3, RISING);
    }
} // namespace Encoder_interrrupts


namespace motor_control
{
    Motor::Motor(int M1, int M2, int speed_pin, int encoder_pin_a, int encoder_pin_b)
    {
        this->M1 = M1;
        this->M2 = M2;
        this->speed_pin = speed_pin;
        this->encoder_pin_a = encoder_pin_a;
        this->encoder_pin_b = encoder_pin_b;

        pinMode(M1, OUTPUT);
        pinMode(M1, OUTPUT);
        pinMode(speed_pin, OUTPUT);
        pinMode(encoder_pin_b, INPUT);
    }

    void Motor::disable()
    {
        digitalWrite(M1, false);
        digitalWrite(M2, false);
    }

    
} // namespace motor_control
