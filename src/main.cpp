#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "FRAM.h"
#include "Accelero.h"
#include "Pitot.h"
#include "radio.h"
#include "define.h"

#include "comm_intern.h"

#define SERIALEXP Serial1

#define SEUIL 10
//VARIABLES COMMUNICATION AVEC CARTE SEQUENCEUR
bool seq_decollage_detect = false;
bool exp_decollage_detect = false;
bool msg_decollage_detect = false;
bool apogee_detectee = false;
bool seq_decollage_detectParExp = false;
bool acquittement_seq_exp = false; //Acquittement de la carte séquenceur envers la carte experience
bool acquittement_exp_seq = false; //Acquittement de la carte experience envers la carte séquenceur

bool acquittement_apogee = false;
bool acquittement_decollage_exp = false;

bool parachute_deploye = false;
bool msg_parachute = false;
int t_msg_seq = 0;
int t_msg_exp = 0;

int msg;
int msgB0;
int msgB1;

//Initialisation des variables
double ax, ay, az, gx, gy, gz, vz;
int  adresse(1), temps(0);

//Variable de detection du décollage

void setup() {
  //Initialisation des communications UART
  Serial_xbee.begin(9600);
  SERIALEXP.begin(9600);

  //Initialisation des différents composants
  MPU_setup(); //accelerometre
  setupFRAM(); //FRAM
  setupPitot();
  temps=micros();
}


void loop() {
  if (micros()-temps>10 && seq_decollage_detect && exp_decollage_detect)
  {
    if (adresse+8>262144)
    {
      while(1){}
      //Arrêt de l'enregistrement si on arrive au bout de la mémoire
    }
    AccelerationReading(ax,ay,az);
    AngularAccelerationReading(gx,gy,gz);
    getspeed(vz);
    writeonFRAM(ax,ay,az,gx,gy,gz,vz,adresse);
    sendalldata(ax,ay,az,gx,gy,gz,vz);
    adresse+=8;
    temps=micros();
  }

  AccelerationReading(ax,ay,az); //Lecture de l'accélération 
  if(az>SEUIL && !exp_decollage_detect || seq_decollage_detect) //Détection du décollage si le seuil est dépassé ou si la carte séquenceur l'indique
  {
    exp_decollage_detect = true;
    seq_decollage_detect = true;
  }


  /* BOUCLES D'EMISSION DE DONNEES VERS CARTE EXPERIENCE */
  if(apogee_detectee && !acquittement_apogee && millis()-t_msg_exp>200){
    //Lorsque l'apogee est detectée, on informe la carte sequenceur jusqu'à obtenir un acquittement de données
    SERIALEXP.write(MARQUEUR);
    SERIALEXP.write(EXP_APOGEE);
    SERIALEXP.write(EXP_APOGEE);
    t_msg_exp = millis();
  }
  if(exp_decollage_detect && !acquittement_decollage_exp && millis()-t_msg_exp>200){
    //Lorsque la carte exp détecte le décollage, on informe la carte séquenceur jusqu'à obtenir un acquittement de données
    SERIALEXP.write(MARQUEUR);
    SERIALEXP.write(EXP_DECOLLAGE);
    SERIALEXP.write(EXP_DECOLLAGE);
    t_msg_exp = millis();
  }
  /* FIN BOUCLES D'EMISSION */

  //FONCTION DE RECEPTION UART / carte seq
  if(SERIALEXP.available()>0){
    msg = SERIALEXP.read();
    if(msg==MARQUEUR){ //On détecte un message lorsque le marqueur de départ est bon
      delay(10);
      msgB0 = SERIALEXP.read();
      msgB1 = SERIALEXP.read();

      if(msgB0 == msgB1){ //le message est validé si les deux octets suivant le marqueur sont identiques
        switch (msgB0)
        {
        case SEQ_DECOLLAGE:
          //Si l'octet de message correspond à une detection de décollage par la carte seq, acquittement des données 
          seq_decollage_detect = true;
          SERIALEXP.write(MARQUEUR);
          SERIALEXP.write(EXP_ACQUITTEMENT);
          SERIALEXP.write(EXP_ACQUITTEMENT);
          break;

        case SEQ_PARACHUTE:
          parachute_deploye = true;
          SERIALEXP.write(MARQUEUR);
          SERIALEXP.write(EXP_ACQUITTEMENT);
          SERIALEXP.write(EXP_ACQUITTEMENT);
          break;

        case SEQ_ACQUITTEMENT_APOGEE:
          acquittement_apogee = true;
          break;
        
        case SEQ_ACQUITTEMENT_DECOLLAGE:
          acquittement_decollage_exp = true;
          break;

        default:
          break;
        }
      }
    }
  }

}