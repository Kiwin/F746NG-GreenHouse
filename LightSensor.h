#ifndef LIGHT_H
#define LIGHT_H
#include "mbed.h"

class LightSensor 
{
    private:
        AnalogIn *lightSens;

    public:
        LightSensor() { this->lightSens = new AnalogIn(A0); }
        float readLight();
};

#endif