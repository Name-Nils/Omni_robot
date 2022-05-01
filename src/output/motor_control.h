#pragma once

namespace motor_control
{
    class Motor
    {
    public:
        // pins
        int encoder_pin_a;
        int encoder_pin_b;
        int motor_pin_a;
        int motor_pin_b;
        int motor_pin_speed;

        // data
        int64_t encoder_pos = 0;
        double rpm_speed = 0;
        double mm_speed = 0;
        double output_position = 0;
        double output_distance = 0;
        float counts_per_rev = 11;
        double gear_ratio = 98.8;             // the gear ratio for this specific motor
        double wheel_circumfrence = PI * 120; // in mm

        // acceleration
        double mm_per_sec = 5;
        double top_speed = 200;
        double current_speed = 0;

    private:
        void acceleration(double to_speed);
        void acceleration() // same function but without the argument
        
        {
            acceleration(top_speed);
        }

    public:
        Motor() = default;
        Motor(const Motor &m);
        Motor(int e_a, int e_b, int m_a, int m_b);


        void update();
        void log();
    };



    Motor::Motor(const Motor &m)
    {
        encoder_pin_a = m.encoder_pin_a; // do not know if these can be accesed at the moment
        encoder_pin_b = m.encoder_pin_b;
        motor_pin_a = m.motor_pin_a;
        motor_pin_b = m.motor_pin_b;

        encoder_pos = m.encoder_pos;
        output_position = m.output_position;
        gear_ratio = m.gear_ratio;
    }
    Motor::Motor(int e_a, int e_b, int m_a, int m_b)
    {
        encoder_pin_a = e_a;
        encoder_pin_b = e_b;
        motor_pin_a = m_a;
        motor_pin_b = m_b;
    }

    void Motor::acceleration(double to_speed)
    {
            uint32_t current_time = millis();
            static uint32_t last_time = current_time;

            if (to_speed > current_speed) /*accelerating*/
            {
                current_speed += ((current_time - last_time) * mm_per_sec) / 1000;
            }
            else if (to_speed < current_speed) /*decelerating*/
            {
            }
            else
            {
                // has hit top speed
            }
            last_time = current_time;
        }

    void Motor::update()
    {
        uint32_t current_time = millis();
        static uint32_t last_time = current_time;
        static double last_rpm = output_position;

        output_position = encoder_pos / (gear_ratio * counts_per_rev);
        output_distance = output_position * wheel_circumfrence;
        rpm_speed = (output_position - last_rpm) / (current_time - last_time) * 60000;
        mm_speed = rpm_speed * wheel_circumfrence;

        last_rpm = output_position;
        last_time = current_time;
    }

    void Motor::log()
    {
        Serial.print("encoder [");
        Serial.print((int32_t)encoder_pos);
        Serial.print("]   output [");
        Serial.print(output_position);
        Serial.print("]   RPM [");
        Serial.print(rpm_speed);
        Serial.print("]   mm/min [");
        Serial.print(mm_speed);
        Serial.println("]");
    }

} // namespace motor_control