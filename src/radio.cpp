#include <Arduino.h>
#include "radio.h"
#include "define.h"


void senddata(double &data, int ID_data)
{
    byte *datab, data0, data1, data2;
    *datab=(byte)data;
    data0=datab[0];
    data1=datab[1];
    data2=datab[2];
    if (Serialxbee.available()>0)
    {
        Serialxbee.write(0x7E);
        Serialxbee.write(0x0);
        Serialxbee.write(0x04);
        Serialxbee.write(ID_data);
        Serialxbee.write(data2);
        Serialxbee.write(data1);
        Serialxbee.write(data0);
        long somme = ID_data + data2 + data1 + data0;
        int checksum = (0xFF - (somme & 0xFF));
        Serialxbee.write(checksum); //checksum   
    }
}

void sendalldata(double &Ax, double &Ay, double &Az, double &Gx, double &Gy, double &Gz, double &Vz)
{
    senddata(Ax, ID_AX);
    senddata(Ay, ID_AY);
    senddata(Az, ID_AZ);
    senddata(Gx, ID_GX);
    senddata(Gy, ID_GY);
    senddata(Gz, ID_GZ);
    senddata(Vz, ID_VZ);

}