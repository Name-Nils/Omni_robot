#pragma once

namespace motor_control
{
    class Motor
    {
    private:
        // private pins
        int M1, M2, speed_pin;

        // private data
        double speed_mm_s;
        double absolute_position_mm;
        uint8_t speed_value = 0;

        // private functions
        void direction(bool direction);
        double confine_encoder_speed(double speed);
        double confine_encoder_absolute(double wanted_pos, double threshold);

    public:
        // public pins
        int encoder_pin_a, encoder_pin_b; // need to be public for the interrupt function that changes the encoder position

        // public data
        volatile int64_t encoder_position = 0;

        // public functions
        Motor(int M1, int M2, int speed_pin, int encoder_pin_a, int encoder_pin_b);


        void print();
        void update_data();
        bool go(double wanted_position);
        void set_speed(double speed);
    };

    inline Motor::Motor(int M1, int M2, int speed_pin, int encoder_pin_a, int encoder_pin_b)
    {
        this->M1 = M1;
        this->M2 = M2;
        this->speed_pin = speed_pin;
        this->encoder_pin_a = encoder_pin_a;
        this->encoder_pin_b = encoder_pin_b;
    }

    inline void Motor::print()
    {
        Serial.print("Pos [");
        Serial.print(absolute_position_mm);
        Serial.print("]   Speed [");
        Serial.print(speed_mm_s);
        Serial.println("]");
    }
    inline void Motor::update_data()
    {
        const double gear_ratio = 98.8;
        const double encoder_reads_per_rotation = 11;
        const double wheel_diameter = 120; // mm

        uint32_t current_time = millis();
        static uint32_t last_time = current_time;
        static double last_position = absolute_position_mm;

        absolute_position_mm = (encoder_position / (gear_ratio * encoder_reads_per_rotation)) * (wheel_diameter * PI);

        speed_mm_s = (absolute_position_mm - last_position) / (current_time - last_time) * 1000;

        last_time = current_time;
        last_position = absolute_position_mm;
    }
    inline void Motor::direction(bool direction)
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
    inline double Motor::confine_encoder_speed(double speed)
    {
        const double P = 1;
        const double I = 0.1;

        uint32_t current_time = millis();
        static uint32_t last_time = current_time;
        static double int_error = 0.0;
        if (int_error == NAN) int_error = 0; // might work to fix the variable and set it to a usable value
        // need a motor control and direction 
        double error = speed_mm_s - speed;
        int_error += error * (double)((current_time - last_time) / 1000.0);

        double pid = error * P + int_error * I;

        Serial.print(error * (double)((current_time - last_time) / 1000.0));
        Serial.print("   ");
        Serial.print(int_error);
        Serial.print("   ");

        last_time = current_time;
    }
    inline double Motor::confine_encoder_absolute(double wanted_position, double threshold)
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
        speed_value += pid;

        analogWrite(speed_pin, speed_value);

        return wanted_position - absolute_position_mm; // returns the distance left to wanted position
    }
    inline bool Motor::go(double wanted_position)
    {
        if (confine_encoder_absolute(wanted_position, 10) <= 10)
            return true;
        return false;
    }
    inline void Motor::set_speed(double speed)
    {
        confine_encoder_speed(speed);
    }
}