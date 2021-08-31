#include "LightSensor.h"

float LightSensor::readLight() 
{
    return this->lightSens->read(); // Below 0.005 in a dark room.
}