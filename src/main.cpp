#include <Arduino.h>
#include "output/motor_control.h"


namespace Encoder_interrrupts
{
  void init();
} // namespace Encoder_interrupts
namespace Calculation
{
  
} // namespace Calculations

void setup()
{
  Serial.begin(9600);
  Encoder_interrrupts::init(); // set the interrupt functions and pins
}

void loop()
{
}