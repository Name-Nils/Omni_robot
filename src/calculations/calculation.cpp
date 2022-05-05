#include <Arduino.h>
#include "Vector.h"
#include "input/remote.h"
#include "output/motor_control.h"

Remote remote(A1, A2, A3, A4, A5);
namespace Motors {extern motor_control::Motor m1, m2, m3;} // namespace Motors

enum State {receiver, usb}; // receiver is the same thing as the remote
State state = receiver; 


namespace Calculation
{
    void calculate_motor_speeds(double angle, double speed, double motors[3])
    {
        // m1 is the front motor (angle = 0), m2 and m3 are +-120 deg

    }


    void remote_control()
    {
        double rotation = 0;
        const double rotation_speed_multiplier = 1;

        uint32_t current_time = millis();
        static uint32_t last_time = current_time;
        uint16_t delta_time = current_time - last_time;
        last_time = current_time;

        remote.update();

        // left stick rotation fixing   this rotation value should be added on to all the motors speed so that the robot will rotate aswell as move in the desired direction
        double add_rotation = (remote.left_stick.size * rotation_speed_multiplier) / delta_time;
        rotation = (remote.left_stick.angle > 90 && remote.left_stick.angle < 270) ? -add_rotation : add_rotation;

        // add the rotation to the motors speed
        double motors[3];
        for (int i = 0; i < 3; i++)
        {
            motors[i] = rotation;
        }

        // right stick movement 
        
    }

    void usb_control()
    {

    }
} // namespace Calculation
