#ifndef ACCELERO_H
#define ACCELERO_H

#include <Wire.h>
#include <Arduino.h>


void MPU_setup();
void AccelerationReading(double &aX, double &aY, double &aZ);
void AngularAccelerationReading(double &gX, double &gY, double &gZ);

#endif