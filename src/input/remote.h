#pragma once

#include <Arduino.h>
#include "Vector.h"

class Input
{
    int pin;
    double data; // stored in variable between 0 and 1 or -1 and 1
    
    const static double deadzone = 0.1;

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
        long read = pulseIn(pin, true, 30000);
        read = read - 1500;
        data = read / 500.0;

        data = (fabs(data) > deadzone) ? data : 0.0;
        return data;
    }
};

enum Stick
{
    forward_reverse,
    left_right,
    turning,
    switch_
};
const int amount_inputs = switch_ + 1;

class Remote
{
public:
    Input inputs[amount_inputs];
    Vector right_stick;
    Vector left_stick;

    Remote(int s1, int s2, int s3, int s4)
    {
        inputs[forward_reverse] = Input(s2);
        inputs[left_right] = Input(s1);
        inputs[turning] = Input(s3);
        inputs[switch_] = Input(s4);
    }
    void update(int amount)
    {
        amount = (amount > amount_inputs) ? amount_inputs : amount;
        for(int i = 0; i < amount; i++) // temp
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
