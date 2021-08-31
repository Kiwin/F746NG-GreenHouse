#include "Humid.h"

float TemperatureSensor::readTemperature(eScale Scale)
{
    this->humidSens->readData();

    while (this->humidSens->readData());

    return this->humidSens->ReadTemperature(Scale);
}