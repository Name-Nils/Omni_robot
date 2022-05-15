#include <Arduino.h>
#include "Vector.h"
#include "input/remote.h"
#include "output/motor_control.h"
#include "../input/usb_control.h"

Remote remote(A7, A6, A5, A4);
namespace Motors {extern motor_control::Motor m1, m2, m3;} // namespace Motors

enum State {receiver, usb}; // receiver is the same thing as the remote
State state = receiver; 

namespace Usb_control
{
    Parsing serial;
} // namespace Usb_control


namespace Calculation
{
    void motor_speed_calc(double wheel_distance, double speed, double angle, double rotation, double* m1, double* m2, double* m3)
    {
        static float m1_pos = PI / 2;
        static float m2_pos = m1_pos + (PI / 3 * 2);
        static float m3_pos = m2_pos + (PI / 3 * 2);

        *m1 = speed * sin(m1_pos - angle) + (wheel_distance * rotation);
        *m2 = speed * sin(m2_pos - angle) + (wheel_distance * rotation);
        *m3 = speed * sin(m3_pos - angle) + (wheel_distance * rotation);
    }

    void remote_control()
    {
        double rotation = 0;
        const double rotation_speed_multiplier = 3;
        const double speed_multiplier = 300;

        remote.update(3);


        // left stick rotation fixing   this rotation value should be added on to all the motors speed so that the robot will rotate aswell as move in the desired direction
        rotation = (remote.left_stick.size * rotation_speed_multiplier);
        rotation = (remote.left_stick.angle > 0) ? rotation : -rotation;

        double speed = remote.right_stick.size;

        double m1, m2, m3;

        motor_speed_calc(110, -speed * speed_multiplier, PI - remote.right_stick.angle, rotation, &m1, &m2, &m3);

        Motors::m1.update_data();
        Motors::m2.update_data();
        Motors::m3.update_data();
        Motors::m1.set_speed(m1);
        Motors::m2.set_speed(m2);
        Motors::m3.set_speed(m3);
    }

    void usb_control()
    {
        static bool init = Usb_control::serial.init();

        static double m1 = 0, m2 = 0, m3 = 0;
        static Usb_control::Parsing last_serial;
        // serial reading and then interpreting
        
        Motors::m1.update_data();
        Motors::m2.update_data();
        Motors::m3.update_data();

        Motors::m1.set_speed(m1);
        Motors::m2.set_speed(m2);
        Motors::m3.set_speed(m3);

        if (Serial.available() == 0) return;

        Usb_control::serial.parse(String(Serial.readStringUntil('\n')).c_str());

        if (
            Usb_control::serial.rotation_wanted == last_serial.rotation_wanted && 
            Usb_control::serial.speed_wanted.angle == last_serial.speed_wanted.angle && 
            Usb_control::serial.speed_wanted.size == last_serial.speed_wanted.size) 
            return;
        last_serial = Usb_control::serial;

        
        motor_speed_calc(110, 
        -Usb_control::serial.speed_wanted.size,
        PI - Usb_control::serial.speed_wanted.angle,
        Usb_control::serial.rotation_wanted, 
        &m1, &m2, &m3);
    }
} // namespace Calculation
