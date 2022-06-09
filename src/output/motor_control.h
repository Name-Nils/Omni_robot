#pragma once

namespace motor_control
{
    struct static_data // is there any other way to have static data in a method? (only did the inheritance to make sure that it was clear that this data is static inside of the methods)
    {
    protected: 
        // all static variables for calculations need to be stored here
            // move speed function
                uint32_t move_speed_last_time = micros();
                double move_speed_last_error = 0; 
                double move_speed_I_error = 0;
            
            // move_absolute function
                uint32_t move_absolute_last_time = micros();
                double move_absolute_current_speed = 0; // speed may not surpass the speed value

            // move_relative function
                double move_relative_last_absolute_position_mm = 0;
                double move_relative_last_delta_position = 0;
    };

    class Motor : public static_data
    {
    private:
        // private pins
        int M1, M2, speed_pin;

        // private data
        double speed_mm_s;
        double absolute_position_mm;
        uint8_t speed_value = 0;

        // private functions
        void direction(bool);
    public:
        // public pins
        int encoder_pin_a, encoder_pin_b; // need to be public for the interrupt function that changes the encoder position

        // public data
        volatile int64_t encoder_position = 0;

        // public functions
        Motor(int M1, int M2, int speed_pin, int encoder_pin_a, int encoder_pin_b);

        void move_speed(bool dir, double speed);
        bool move_relative(double delta_position, double speed, double threshold, double accel);
        bool move_absolute(double wanted_pos, double speed, double threshold, double accel); // will return true if is at position
        void disable();
    };
}