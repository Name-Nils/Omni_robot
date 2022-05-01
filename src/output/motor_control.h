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
        double confine_encoder(double wanted_pos, double threshold);
    public:
        // public pins
        int encoder_pin_a, encoder_pin_b;

        // public data
        int64_t encoder_position = 0;
        

        // public functions
        Motor(int M1, int M2, int speed_pin, int encoder_pin_a, int encoder_pin_b);
        
        void print();
        void update_data();
        bool go(double wanted_position);
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
        const double wheel_diameter = 120; //mm


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
    inline double Motor::confine_encoder(double wanted_position, double threshold)
    {
        // need to adjust the speed in small amounts so get to the position correctly. speed value is a 8 bit integer (pwn signal)
        direction(wanted_position > absolute_position_mm);

        if (abs(absolute_position_mm - wanted_position) > threshold)
        {
            // shoud move according to this function
            speed_value = (speed_value < 255 - 50) ? speed_value + 50 : 255;
        }
        else
        {
            speed_value = 0;
        }

        analogWrite(speed_pin, speed_value);

        return wanted_position - absolute_position_mm; // returns the distance left to wanted position
    }
    inline bool Motor::go(double wanted_position)
    {
        if (confine_encoder(wanted_position, 10) <= 10) return true;
        return false;
    }
}