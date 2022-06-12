#include <Arduino.h>
#include "Vector.h"
#include "input/remote.h"
#include "output/motor_control.h"
#include "../input/usb_control.h"
#include "position.h"

namespace Usb
{
    extern Usb_control::Parsing control;
} // namespace Usb_control
namespace Motors
{
    extern motor_control::Motor m1, m2, m3;
} // namespace Motor


namespace Calculation
{
    void motor_speed_calc(double wheel_distance, double speed, double angle, double rotation, double *m1, double *m2, double *m3)
    {
        static float m1_pos = PI / 2;
        static float m2_pos = m1_pos + (PI / 3 * 2);
        static float m3_pos = m2_pos + (PI / 3 * 2);

        *m1 = speed * sin(m1_pos - angle) + (wheel_distance * rotation);
        *m2 = speed * sin(m2_pos - angle) + (wheel_distance * rotation);
        *m3 = speed * sin(m3_pos - angle) + (wheel_distance * rotation);
    }

    void usb_control()
    {
        static Position pos;
        static bool at_posisiton = true;
        static bool new_move = true;

        Motors::m1.update_data();
        Motors::m2.update_data();
        Motors::m3.update_data();        

        if (Serial.available() > 0)
        {
            at_posisiton = false;
            new_move = true;

            const int buffer_size = 20;
            byte buffer[buffer_size];
            size_t size = Serial.readBytesUntil('\n', buffer, buffer_size);

            Usb::control.parse((const char *)buffer, size);
        }
  

        // if the robot rotates then the absolute coordinates cant be calculated only from the absolute rotation of the motors
        double *movement = Usb::control.get_movement(); // movement in x and y coordinates

        bool *settings = Usb::control.get_settings();
        static Usb_control::Settings last_setting;
        if (settings[Usb_control::ABSOLUTE])
        {
            if (last_setting != Usb_control::ABSOLUTE) 
            {
                Motors::m1.reset_data(); 
                Motors::m2.reset_data(); 
                Motors::m3.reset_data(); 
                pos.reset();
            }
            if (at_posisiton) // otherwise the function will repeatetely send ok and clog the system
            {
                Motors::m1.disable(); // motors should not move if at position
                Motors::m2.disable();
                Motors::m3.disable();
                return;
            }

            static Vector last_pos = Vector(0, 0, true);
            
            double x = movement[Usb_control::X];
            double y = movement[Usb_control::Y];


            Vector move = Vector(x - last_pos.x, y - last_pos.y, true);
            double m1, m2, m3;
            motor_speed_calc(110.0, move.size, move.angle, 0.0, &m1, &m2, &m3);

            // the fastest accelerator needs to be the motor that moves the fastest and the rest are products of that acceleration
            double fastest = 0;
            if (fabs(m1) > fastest) fastest = fabs(m1);
            if (fabs(m2) > fastest) fastest = fabs(m2);
            if (fabs(m3) > fastest) fastest = fabs(m3);

            double m1_multi = m1 / fastest;
            double m2_multi = m2 / fastest;
            double m3_multi = m3 / fastest;

            double accel = movement[Usb_control::ACCEL];

            bool m1_ans = Motors::m1.move_relative(m1, m1_multi * movement[Usb_control::SPEED], 10, m1_multi * accel, new_move);
            bool m2_ans = Motors::m1.move_relative(m2, m2_multi * movement[Usb_control::SPEED], 10, m2_multi * accel, new_move); // not sure if this is the correct way to perform this movement with the
            bool m3_ans = Motors::m1.move_relative(m3, m3_multi * movement[Usb_control::SPEED], 10, m3_multi * accel, new_move); // the acceleration and speed being multiplied by the multi means that they should all atain there terminal velocity at the same time

            if (m1_ans && m2_ans && m3_ans)
            {
                at_posisiton = true;
                last_pos = Vector(x,y,true);
                pos.x = x;
                pos.y = y;

                Serial.println(String("OK"));
            }

            last_setting = Usb_control::ABSOLUTE;
            new_move = false;
        }
        else if (settings[Usb_control::RELATIVE])
        {
            if (last_setting != Usb_control::RELATIVE)
            {
                Motors::m1.reset_data(); 
                Motors::m2.reset_data(); 
                Motors::m3.reset_data(); 
                pos.reset();
            }
            if (at_posisiton)
            {
                Motors::m1.disable();
                Motors::m2.disable();
                Motors::m3.disable();
                return;
            }

            

            last_setting = Usb_control::RELATIVE;
        }
        else if (settings[Usb_control::VELOCITY])
        {
            if (last_setting != Usb_control::VELOCITY) Motors::m1.reset_data(); Motors::m2.reset_data(); Motors::m3.reset_data();
            
            Vector move = Vector(movement[Usb_control::X], movement[Usb_control::Y], true);
            double rotation = movement[Usb_control::ROTATION];
            double m1, m2, m3;

            motor_speed_calc(110.0, move.size, move.angle, rotation, &m1, &m2, &m3);
            Motors::m1.move_speed(m1 > 0.0, m1);
            Motors::m1.move_speed(m2 > 0.0, m2);
            Motors::m1.move_speed(m3 > 0.0, m3);

            last_setting = Usb_control::VELOCITY;
        }
        else if (settings[Usb_control::MOTOR])
        {
            // this will not work at the moment since every motor requires alot of data to move and there arent at the moment enough variabled being read from serial to store all that data

            last_setting = Usb_control::MOTOR;
        }

        if (settings[Usb_control::GETPOS])
        {
            settings[Usb_control::GETPOS] = false;
            // send the position of the robot, this posiiton is relative to that last time the posisition variable changed
            Serial.println(pos.string(Coordinate::CARTESIAN, false, 5));
        }
    }
} // namespace Calculation
