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
namespace Calculation
{
  void remote_control();
  void usb_control();
} // namespace Calculations

void setup()
{
  Serial.begin(9600);
  Encoder_interrrupts::init(); // set the interrupt functions and pins
}

void loop()
{
  if (digitalRead(A4))
  {
    Calculation::remote_control();
  }
  else
  {
    Calculation::usb_control();
  }
}