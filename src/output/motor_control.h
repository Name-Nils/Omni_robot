#pragma once
#include <Arduino.h>

class Motor
{
public:
    // pins
    int encoder_pin_a;
    int encoder_pin_b;
    int motor_pin_a;
    int motor_pin_b;

    // data
    int64_t encoder_pos = 0;
    double output_position = 0;
    float counts_per_rev = 11;
    double gear_ratio = 98.8; // the gear ratio for this specific motor

    Motor() = default;
    Motor(const Motor &m)
    {
        encoder_pin_a = m.encoder_pin_a; // do not know if these can be accesed at the moment
        encoder_pin_b = m.encoder_pin_b;
        motor_pin_a = m.motor_pin_a;
        motor_pin_b = m.motor_pin_b;

        encoder_pos = m.encoder_pos;
        output_position = m.output_position;
        gear_ratio = m.gear_ratio;
    }
    Motor(int e_a, int e_b, int m_a, int m_b)
    {
        encoder_pin_a = e_a;
        encoder_pin_b = e_b;
        motor_pin_a = m_a;
        motor_pin_b = m_b;
    }

    void update()
    {
        output_position = encoder_pos / (gear_ratio * counts_per_rev);
    }

    void log()
    {
        Serial.print("encoder [");
        Serial.print((int32_t)encoder_pos);
        Serial.print("]   output [");
        Serial.print(output_position);
        Serial.println("]");
    }
};

class Omni_Motors
{
public:
    int hello = 5;
    Motor m1, m2, m3;

    Omni_Motors() = default;
    Omni_Motors(const Omni_Motors &o)
    {
        m1 = o.m1;
        m2 = o.m2;
        m3 = o.m3;
    }
    Omni_Motors(Motor m1, Motor m2, Motor m3)
    {
        this->m1 = m1;
        this->m2 = m2;
        this->m3 = m3;
    }

    void init()
    {

        // attachInterrupt(digitalPinToInterrupt(m1.encoder_pin_a), update_encoder<m1>, )
    }
};

Omni_Motors motors();

namespace Encoder_interrrupts
{
    void m1()
    {

        bool current_a = digitalRead(motors.m1.encoder_pin_a);
        static bool last_a = current_a;

        if ((last_a == false) && (current_a == true))
        {
            if (digitalRead(m->encoder_pin_b) == false)
            {
                m->encoder_pos--;
            }
            else
            {
                m->encoder_pos++;
            }
        }
        last_a = current_a;
    }
} // namespace Encoder_interrrupts