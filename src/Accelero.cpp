#include "Accelero.h"
//BASE ON:
//Arnaud Schlumberger : AcceleroTest        ///   
//Last revision: 23 November 2019///
////////////////////////////////////




///Arduino Nano:
//SDA = A4;
//SCL = A5;
////(Same on most Arduinos)
///MPU-6050
//I2C Address: 1101000 (binary), 0x68 (hexa) when AD0 = 0 logic, 
//I2C Address: 1101001 when AD0 = 1 logic
//////IMPORTANT: When modifying Full Scale Range, be sure to change the corresponding sensor sensitivity

#define MPU_addr 0x68

void MPU_setup() {
  //Module is in sleep mode on startup, we then need to get out of sleep mode.
  //According to the register map, this can be done by modifying the Power Management Register, which address is 6B(Hex);107(Deci) [See 4.28/Register Map]
  //Bit 6 has to be 0
  Wire.beginTransmission(MPU_addr);//Initializing communication to MPU with the specified address
  Wire.write(0x6B); //Selecting the Power Management register
  Wire.write(0b00000000); //Changing values of the register (No Reset|No Sleep Mode|Internal clock|\|Temperature Sensor Disabled|Internal clock)
  Wire.endTransmission(MPU_addr);
  //  We then need to specify the full scale range (max and min values) of the gyroscope.
  //  The Gyroscope Configuration Register has the address 1B(Hex);27(Deci) [See 4.4/Register Map]
  //  We are going to disable self-test (bit 7,6 and 5 to 0) and set the Full Scale Range at +-250°/s(bit 4 and 3 to 0), following the same procedure as above.
  //////IMPORTANT: When modifying Full Scale Range, be sure to change the corresponding sensor sensitivity
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1B); //Selecting Gyro Config Register
  Wire.write(MPU_addr);
  Wire.endTransmission();
  //Same thing for the Accelerometer. Setting Full Scale to +-2g. Register address is 1C(Hex), 20(Deci) [See 4.5/Register Map]
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission();

  
}

void AccelerationReading(double &aX, double &aY, double &aZ){
    
  //Accelerometer Measurements are stored in Registers 59(3B[hex]) to 64(40[hex]), there are 2 registers per axis -> 2 byte/axis
  Wire.beginTransmission(MPU_addr); //Telling the MPU what registers we want to receive -> first register in the row
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU_addr,6); //3axis*2bits=6bytes requested
  aX = Wire.read()<<8|Wire.read();
  aY = Wire.read()<<8|Wire.read();
  aZ = Wire.read()<<8|Wire.read();
}

void AngularAccelerationReading(double &gX, double &gY, double &gZ){

  //Gyroscope measurements stored in registers 43(dec;67[hex]) to 48(dec;72[hex]), 2 registers per axis  -> 2bytes/axis
  //Same thing as in above function
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43);
  Wire.endTransmission(MPU_addr);
  Wire.requestFrom(MPU_addr,6);
  gX = Wire.read()<<8|Wire.read();
  gY = Wire.read()<<8|Wire.read();
  gZ = Wire.read()<<8|Wire.read();
}