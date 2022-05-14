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
} // namespace Calculations


void setup()
{
  Serial.begin(9600);
  Encoder_interrrupts::init(); // set the interrupt functions and pins
}

void loop()
{/*
  Motors::m1.update_data();
  Motors::m2.update_data();
  Motors::m3.update_data();


  static double positions[5];
  static int index = 0;
  positions[index] = map(pulseIn(A7, HIGH, 30000), 1000, 2000, -150, 150);
  index ++;
  index = (index >= 5) ? 0 : index;

  double wantes_position = 0;
  for (int i = 0; i < 5; i++)
  {
    wantes_position += positions[i];
  }
  wantes_position /= 5;

  Serial.println(wantes_position);
  Motors::m1.set_speed(wantes_position);
  Motors::m2.set_speed(wantes_position);
  Motors::m3.set_speed(wantes_position);
*/ 
  
  Calculation::remote_control();
}