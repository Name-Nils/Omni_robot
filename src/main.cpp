#include <Arduino.h>
#include "output/motor_control.h"


namespace Encoder_interrrupts
{
  void init();
} // namespace Encoder_interrupts
namespace Motors
{
  extern motor_control::Motor m1,m2,m3;
} // namespace Motor

namespace Calculation
{
  void usb_control();
} // namespace Calculations

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(50);
  Encoder_interrrupts::init(); // set the interrupt functions and pins
}

void loop()
{
  Calculation::usb_control();
}