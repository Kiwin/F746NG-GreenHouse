#ifndef HUMID_H
#define HUMID_H
#include "DHT.h"

class TemperatureSensor 
{
    private:
        DHT *humidSens;

    public:
        TemperatureSensor(PinName pin) 
        { 
            //Set pins of the humid sensor.
            this->humidSens = new DHT(pin, AM2302); 
        }
        
        float readTemperature(eScale Scale);
};

#endif