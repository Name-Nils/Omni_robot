#include <Arduino.h>
#include "Vector.h"
#include "input/remote.h"
#include "output/motor_control.h"
#include "../input/usb_control.h"

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

    
} // namespace Calculation
