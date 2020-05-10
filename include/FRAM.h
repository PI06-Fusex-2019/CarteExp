#ifndef FRAM_H
#define FRAM_H

#include <SPI.h>
#include <Arduino.h>
#include "define.h"

//Codes de commandes de la FRAM
#define WRITE_en 0b00000110 //autorise l'ecriture
#define WRITE_dis 0b00000100 //enleve l'autorisation d'ecriture
#define READ_stat 0b00000101 
#define WRITE_stat 0b00000001
#define READ 0b00000011
#define WRITE 0b00000010
#define get_ID  0b10011111 //donne les ID de la FRAM

#define emptybyte 0b00000000

void setupFRAM();
bool checkID();

void writeonFRAM(double &Ax,double &Ay,double &Az, double &Gx,double &Gy,double &GZ,double &vz,int adresse);
// void writeOnFile(); //Ca va dépendre de comment on code l'info

#endif