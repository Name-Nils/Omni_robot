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

  analogWrite(10, 255);
  digitalWrite(9, true);
  digitalWrite(8, false);
}

void loop()
{
  Motors::m1.update_data();
/*
  analogWrite(10, 200);

  digitalWrite(9, true);
  digitalWrite(8, false);
delay(2000);
analogWrite(10, 255);
  digitalWrite(9, false);
  digitalWrite(8, true);
delay(2000);
*/}