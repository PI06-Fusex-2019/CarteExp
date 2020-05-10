#include "Pitot.h"
#include <Arduino.h>

void setupPitot()
{
    pinMode(PitotOutput, INPUT);
}
void getspeed(double &v)
{
    double Vout(0), pressure(0);
    Vout=analogRead(PitotOutput);
    pressure=10*abs(Vout-2.5);

    v=sqrt(2*pressure/1.292);  
}