#include <Arduino.h>
#include "output/motor_control.h"

namespace Motors
{
    extern motor_control::Motor m1, m2, m3;
} // namespace Motors
namespace Encoder_interrrupts
{
  void init();
} // namespace Encoder_interrupts

void setup()
{
  Serial.begin(9600);
  Encoder_interrrupts::init(); // set the interrupt functions and pins
}

void loop()
{
  Motors::m1.update_data();

  double wantes_position = map(analogRead(A7), 0, 1024, -150, 150);
  Motors::m1.set_speed(wantes_position);
  

  delay(10);
}