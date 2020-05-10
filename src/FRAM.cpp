#include "FRAM.h"


byte ID1(5),ID2(5),ID3(5),data(5);

//Initialisation des pins de commande et de traitement des données
void setupFRAM()
{

  pinMode(FRAM_MISO, OUTPUT);
  pinMode(FRAM_MOSI, INPUT);
  pinMode(FRAM_SCK,OUTPUT);
  pinMode(FRAM_CS, OUTPUT);    
  digitalWrite(FRAM_CS,HIGH);
}

//On vérifie si on detecte le bon composant
bool checkID()
{
    digitalWrite(FRAM_CS,LOW); //Permet une instruction
    SPI.begin();

    SPI.transfer(get_ID);
    ID1 = SPI.transfer(emptybyte);
    ID2 = SPI.transfer(emptybyte);
    ID3 = SPI.transfer(emptybyte);

    SPI.end();
    digitalWrite(FRAM_CS,HIGH);

    return (ID1==0b10101110 && ID2==0b10000011 && ID==0b00011010);
}

void writeonFRAM(double &Ax,double &Ay,double &Az, double &Gx,double &Gy,double &Gz,double &Vz,int adresse)
{
  byte *ad,ad0,ad1,ad2,ad3;
  *ad=(byte)adresse;
  ad0=ad[0];
  ad1=ad[1];
  ad2=ad[2];
  ad3=ad[3];
  SPI.begin();
  //SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(FRAM_CS,LOW);
  SPI.transfer(WRITE_en);
  SPI.transfer(ad3);
  SPI.transfer(ad2);
  SPI.transfer(ad1);
  SPI.transfer(ad0);
  SPI.transfer(Ax);
  SPI.transfer(Ay);
  SPI.transfer(Az);
  SPI.transfer(Gx);
  SPI.transfer(Gy);
  SPI.transfer(Gz);
  SPI.transfer(Vz);

  //On peut écrire autant que l'on veut tant qu'on a pas fait la commande suivante (attention à ne pas écraser de donnée déjà existante):
  digitalWrite(FRAM_CS,HIGH);
  SPI.transfer(WRITE_dis);
  SPI.end();
}