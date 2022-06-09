#include <Arduino.h>
#include "motor_control.h"


namespace Helper
{
    template<typename gen>
    gen clamp(gen &val, gen min, gen max)
    {
        val = (val > max) ? max : val;
        val = (val < min) ? min : val;
        return val;
    }
} // namespace Helper


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

    void Motor::direction(bool dir)
    {
        if (dir)
        {
            digitalWrite(M1, true);
            digitalWrite(M2, false);
        }
        else
        {
            digitalWrite(M1, false);
            digitalWrite(M2, true);
        }
    }

    void Motor::disable()
    {
        digitalWrite(M1, false);
        digitalWrite(M2, false);
    }

    void Motor::move_speed(bool dir, double speed)
    {
        // this is a pid controlled speed setting
        const double P = 0.6;
        const double I = 1.0;
        const double D = 0.0;

        uint32_t current_time = micros();
        double delta_seconds = (current_time - move_speed_last_time) / 1.0e6;
        if (delta_seconds > 1.0) delta_seconds = 0.0; // in case of non use of this function the delta seconds will be reset to 0

        double pid = 0;
        double P_error = speed - speed_mm_s;
        move_speed_I_error += P_error * delta_seconds;
        double D_error = 0; 
        if (P_error - move_speed_last_error != 0.0) D_error = (P_error - move_speed_last_error) / delta_seconds;
        move_speed_last_error = P_error;

        pid = P_error * P + move_speed_I_error * I + D_error * D;
        speed_value = (int)round(fabs(Helper::clamp(pid, -255.0, 255.0)));

        direction(pid > 0.0);
        analogWrite(speed_pin, speed_value);
    }

    bool Motor::move_absolute(double wanted_pos, double speed, double threshold, double acceleration)
    {
        // everything is in mm and seconds (wanted_pos is in mm and accereration is mm/s^2)
        uint32_t current_time = micros();
        double delta_seconds = (current_time - move_absolute_last_time) / 1.0e6; // so that the number is in seconds
        move_absolute_last_time = micros(); // after using it to calculate the diff it can be restored to the current time

        double amount_left = wanted_pos - absolute_position_mm;
        if (fabs(amount_left) < threshold) return true;

        double dist_to_decelerate = pow(move_absolute_current_speed, 2) / (2 * acceleration);
        if (dist_to_decelerate > fabs(amount_left))
        {
            // calculate the speed that should be had at this point
            move_absolute_current_speed = sqrt(2 * acceleration * fabs(amount_left));
            Helper::clamp(move_absolute_current_speed, 0.0, speed);
        }
        else
        {
            move_absolute_current_speed += delta_seconds * acceleration;
            Helper::clamp(move_absolute_current_speed, 0.0, speed);
        }

        move_speed(amount_left > 0.0, move_absolute_current_speed);
        return false;
    }

    bool Motor::move_relative(double delta_position, double speed, double threshold, double accel)
    {
        if (move_relative_last_delta_position != delta_position)
        {
            move_relative_last_absolute_position_mm = absolute_position_mm;
        }
        move_absolute(delta_position + move_relative_last_absolute_position_mm, speed, threshold, accel);
        
        move_relative_last_delta_position = delta_position;
    }
} // namespace motor_control
