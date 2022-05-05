#pragma once

#include <Arduino.h>
#include "Vector.h"

class Input
{
    int pin;
    double data; // stored in variable between 0 and 1 or -1 and 1

    uint16_t timeout = 30000;
    uint16_t largest = 20000;
    uint16_t smallest = 10000;
    uint16_t divide = 10000;

public:
    Input() = default;
    Input(const Input &i)
    {
        pin = i.pin;
        data = i.data;
    }
    Input(int pin)
    {
        this->pin = pin;
    }

    double get()
    {
        return data;
    }
    double update()
    {
        unsigned long read = pulseIn(pin, true, timeout);
        read += -(((largest - smallest) / 2) + smallest);
        read /= divide;
        data = read;
        return data;
    }
};

enum Stick
{
    forward_reverse,
    left_right,
    turning,
    switch_one,
    switch_two
};
const int amount_inputs = switch_two;

class Remote
{
public:
    Input inputs[amount_inputs];
    Vector right_stick;
    Vector left_stick;

    Remote(int s1, int s2, int s3, int s4, int s5)
    {
        inputs[forward_reverse] = Input(s1);
        inputs[left_right] = Input(s2);
        inputs[turning] = Input(s3);
        inputs[switch_one] = Input(s4);
        inputs[switch_two] = Input(s5);
    }
    void update()
    {
        for(int i = 0; i < amount_inputs; i++)
        {
            inputs[i].update();
        }

        double right_y = inputs[forward_reverse].get();
        double right_x = inputs[left_right].get();
        right_stick = Vector(atan2(right_y, right_x), sqrt(pow(right_x, 2) + pow(right_y, 2)));

        double left_y = 0;
        double left_x = inputs[turning].get();
        left_stick = Vector(atan2(left_y, left_x), sqrt(pow(left_x, 2) + pow(left_y, 2)));
    }
};
