#include <Arduino.h>
#include "output/motor_control.h"

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  m.update();
  m.log();
}