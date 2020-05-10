#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "FRAM.h"
#include "Accelero.h"
#include "Pitot.h"
#include "radio.h"
#include "define.h"

void setup() {
  //Initialisation des communications UART
  Serial_xbee.begin(9600);

  //Initialisation des différents composants
  MPU_setup(); //accelerometre
  setupFRAM(); //FRAM
  setupPitot();
}

//Initialisation des variables
double ax, ay, az, gx, gy, gz, vz;
int  adresse(1);

void loop() {
  AccelerationReading(ax,ay,az);
  AngularAccelerationReading(gx,gy,gz);
  getspeed(vz);
  writeonFRAM(ax,ay,az,gx,gy,gz,vz,adresse);
  sendalldata(ax,ay,az,gx,gy,gz);
  adresse+=8;
}