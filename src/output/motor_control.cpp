#include <Arduino.h>
#include "motor_control.h"

namespace Motors
{
    motor_control::Motor m1(8, 9, 10, 2, 3), m2(0, 0, 0, 4, 5), m3(0, 0, 0, 6, 7);
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
    }

    void Motor::print()
    {
        Serial.print("Pos [");
        Serial.print(absolute_position_mm);
        Serial.print("]   Speed [");
        Serial.print(speed_mm_s);
        Serial.println("]");
    }
    void Motor::update_data()
    {
        const double gear_ratio = 98.8;
        const double encoder_reads_per_rotation = 11;
        const double wheel_diameter = 120; // mm

        uint32_t current_time = micros();
        static uint32_t last_time = current_time;
        static double last_position = absolute_position_mm;

        absolute_position_mm = (encoder_position / (gear_ratio * encoder_reads_per_rotation)) * (wheel_diameter * PI);


        const int avg_size = 4;
        static double speed_smoothing[avg_size];
        static int avg_index = 0;
        speed_smoothing[avg_index] = (absolute_position_mm - last_position) / (current_time - last_time) * 1.0e6;
        avg_index ++;
        if (avg_index >= avg_size) avg_index = 0;

        double avg_total = 0;
        for (int i = 0;  i < avg_size; i ++)
        {
            avg_total += speed_smoothing[i];
        }
        speed_mm_s = avg_total / avg_size;


        last_time = current_time;
        last_position = absolute_position_mm;
    }
    void Motor::direction(bool direction)
    {
        if (direction)
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
    double Motor::confine_encoder_speed(double speed)
    {
        const double P = 1.5;
        const double I = 3.0;
        const double D = 0.0;

        uint32_t current_time = micros();
        static uint32_t last_time = current_time;
        double delta_seconds = (current_time - last_time) / 1.0e06;
        last_time = current_time;
        
        static double last_error = 0;
        if (isnan(last_error)) last_error = 0;
        static double I_error = 0;
        if (isnan(I_error)) I_error = 0;

        double P_error = speed - speed_mm_s;
        I_error += P_error * delta_seconds;
        double D_error = 0;
        if (P_error - last_error != 0.0) D_error = (P_error - last_error) / delta_seconds;
        last_error = P_error;


        double pid = P_error * P + I_error * I + D * D_error;
        speed_value = fabs(pid);
        speed_value = (speed_value > 255.0) ? 255.0 : speed_value;

        direction(pid > 0.0);
        analogWrite(speed_pin, speed_value);
    }
    double Motor::confine_encoder_absolute(double wanted_position, double threshold)
    {
        if (abs(wanted_position - absolute_position_mm) <= threshold) 
        {
            speed_value = 0;
            analogWrite(speed_pin, speed_value);
            return wanted_position - absolute_position_mm;
        }

        // Pid
        const double P = 1.5;
        const double I = 0.05;
        static double integral_error = 0;
        uint32_t current_speed = millis();
        static uint32_t last_speed = current_speed;

        double error = wanted_position - absolute_position_mm;
        integral_error += error * (current_speed - last_speed) / 1000;
        double pid = error * P + integral_error * I;

        last_speed = current_speed;

        direction(pid > 0);
        speed_value = pid;

        analogWrite(speed_pin, speed_value);

        return wanted_position - absolute_position_mm; // returns the distance left to wanted position
    }
    bool Motor::go(double wanted_position)
    {
        if (confine_encoder_absolute(wanted_position, 10) <= 10)
            return true;
        return false;
    }
    void Motor::set_speed(double speed)
    {
        confine_encoder_speed(speed);
    }
} // namespace motor_control
