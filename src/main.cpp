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

  double wantes_position = map(analogRead(A7), 0, 1024, -1000, 1000);
  Motors::m1.go(wantes_position);
  Serial.print(wantes_position);
  Serial.print("   ");  
  Serial.print(Motors::m1.the_thing());
  //Motors::m1.print();

  Serial.print("   ");
  Serial.println(Motors::m1.the_otha_one());
}