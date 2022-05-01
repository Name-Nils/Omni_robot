#include <Arduino.h>
#include "Vector.h"
#include "input/remote.h"
#include "output/motor_control.h"

Remote remote(A1, A2, A3, A4, A5);
namespace Motors {extern motor_control::Motor m1, m2, m3;} // namespace Motors
