#pragma once

namespace motor_control
{
    struct static_data // is there any other way to have static data in a method? (only did the inheritance to make sure that it was clear that this data is static inside of the methods)
    {
    protected: 
        // all static variables for calculations need to be stored here
        
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
        void direction(bool direction);

    public:
        // public pins
        int encoder_pin_a, encoder_pin_b; // need to be public for the interrupt function that changes the encoder position

        // public data
        volatile int64_t encoder_position = 0;

        // public functions
        Motor(int M1, int M2, int speed_pin, int encoder_pin_a, int encoder_pin_b);

        void disable();
    };
}