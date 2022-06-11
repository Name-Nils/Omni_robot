#pragma once


namespace Helper
{
    template<typename gen, int length>
    struct Average
    {
        int i = 0;
        gen data[length];
        Average() = default;
        Average(gen default_val)
        {
            for (int i = 0; i < length; i++)
            {
                data[i] = default_val;
            }
        }
        gen push(gen val)
        {
            data[i] = val;
            i++;
            gen sum = 0;
            for (int v = 0; v < length; v++)
            {
                sum += data[v];
            }
            return sum / length;
        }
        gen get()
        {
            gen sum = 0;
            for (int v = 0; v < length; v++)
            {
                sum += data[v];
            }
            return sum / length;
        }
    };
} // namespace Helper


namespace motor_control
{
    struct static_data // is there any other way to have static data in a method? (only did the inheritance to make sure that it was clear that this data is static inside of the methods)
    {
    protected: 
        // all static variables for calculations need to be stored here
            // update_data function
                uint32_t update_data_last_time = micros();
                Helper::Average<double, 4> update_data_speed_smoothing = Helper::Average<double, 4>(0.0); // this should work although i have had problems with this for some reason in the past
                double update_data_last_position = 0;

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

        void update_data();
        void move_speed(bool dir, double speed);
        bool move_relative(double delta_position, double speed, double threshold, double accel, bool new_move);
        bool move_absolute(double wanted_pos, double speed, double threshold, double accel, bool new_move); // will return true if is at position
        void reset_data();
        void disable();
    };
}