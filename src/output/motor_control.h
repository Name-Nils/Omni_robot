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


        // private functions
        double confine_encoder(bool direction, double wanted_pos);
    public:
        // public pins
        int encoder_pin_a, encoder_pin_b;

        // public data
        int64_t encoder_position = 0;
        

        // public functions
        Motor(int M1, int M2, int speed_pin, int encoder_pin_a, int encoder_pin_b);
        
        void Motor::update_data();
    };
    

    inline Motor::Motor(int M1, int M2, int speed_pin, int encoder_pin_a, int encoder_pin_b) 
    {
        this->M1 = M1;
        this->M2 = M2;
        this->speed_pin = speed_pin;
        this->encoder_pin_a = encoder_pin_a;
        this->encoder_pin_b = encoder_pin_b;
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
    inline double Motor::confine_encoder(bool direction, double wanted_pos)
    {

    }
}