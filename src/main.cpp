/* Copyright (C) 2022 Alain Dube
 * All rights reserved.
 *
 * Projet Stone de base
 * Ecole du Web
 * Cours Systèmes embarqués (c)2022
 *
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 22/08/15
    @description
      Démonstration comment utiliser le PORT SERIE pour accèder aux fonctionnalités
      de l'écran STONE en utilisant la classe MyStone et MySerial

    platform = ESP32
    OS : Arduino
    Langage : C++

    Historique des versions
        Version    Date       Auteur       Description
        1.1        22/08/15  Alain       Première version du logiciel

    Fonctionnalités implantées
        Lecture des evénements envoyés par l'écran
        Envoyer une commande à l'écran
          Optenir la version du Firmware de l'écran
 * */

#include <Arduino.h>

#define RXD2 18//16
#define TXD2 19//17
#define BAUD_RATE 115200

#include <iostream>

/**
 * Nicolas Boisclair
 */
#include "MyTemp.h"
MyTemp *myTemp = new MyTemp();

#include "MyButton.h"
MyButton *myButtonT4 = new MyButton();
MyButton *myButtonT5 = new MyButton();

#include "MyStone.h"
MyStone *myStone;

std::string intToHexa(int value){
  char buffer[10];
  sprintf(buffer , "0x%4X", value);
  return (buffer);
};

std::string charToString(char str, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), str);
  return (buffer);
};

std::string intToString(int value, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), value);
  return (buffer);
};


// Variable contenant le nom du bois
std::string boisNom = "";

// Variable contenant le compteur du temps de séchage
int compteurTempsSechage = 0;

// Compteur en string
char strCompteurTempsSechage[64];

// Variable contenant le temps de sechage
int tempsSechage = 0;

// Temperature minimum
float temperatureMin = 0;

// Température capté
float temperature = 0;

// Variable représentant si le four est partie ou pas
int fourEnCuisson = 0;


//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {
  datasRead rd = myStone->getValidsDatasIfExists();
  //std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
  switch(rd.id) {

      case 0x0002: { //Version
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneVersion = rd.name;
          std::cout << "Version : " <<  stoneVersion.c_str() << "\n";

          //std::this_thread::sleep_for(std::chrono::milliseconds(10));

          break;
          }
        case 0x1001: { // Bouton
            
            std::string stoneNom = rd.name;
            std::string nomBouton = "btndemarragefour";
            
            if (stoneNom == nomBouton && rd.type == 1)
            {
                std::cout << "Démarrage du four" << std::endl;
                std::cout << boisNom << std::endl;
                if (boisNom == "Erable")
                {
                    tempsSechage = 20;
                    temperatureMin = 25;
                    fourEnCuisson = 1;
                }
                else if(boisNom == "Saule")
                {
                    tempsSechage = 10;
                    temperatureMin = 24;
                    fourEnCuisson = 1;
                }
                else if(boisNom == "Chene")
                {
                    tempsSechage = 15;
                    temperatureMin = 24;
                    fourEnCuisson = 1;
                }
                else if(boisNom == "Acajou")
                {
                    tempsSechage = 18;
                    temperatureMin = 23;
                    fourEnCuisson = 1;
                }
            }
            break;
        }
        case 0x10B0: { // Combo box
            std::string stoneNom = rd.name;
            std::string stoneNomBois = stoneNom.substr(15, (stoneNom.length() - 15));
            std::string nomBouton = "btndemarragefour";
            
            if (stoneNomBois == "Erabl")
            {
                myStone->setLabel("lblsechage", "");
                boisNom = "Erable";
                std::cout << "Bois Erable choisi" << std::endl;
                myStone->setLabel("lblbois", "Erable");
                myStone->setLabel("lbltype", "Dur");
                myStone->setLabel("lblorigine", "Canada");
                myStone->setLabel("lbltempsechage", "20 secondes");
                myStone->setLabel("lbltemperaturemin", "25 Celcius");
                myStone->setLabel("lblboisfour", "Erable");
                myStone->setLabel("lbltemprestant", "0/20s");
                myStone->setLabel("lbltemperatureminfour", "(min : 25 Celcius)");
            }
            else if (stoneNomBois == "Saul")
            {
                myStone->setLabel("lblsechage", "");
                boisNom = "Saule";
                std::cout << "Bois Saule choisi" << std::endl;
                myStone->setLabel("lblbois", "Saule");
                myStone->setLabel("lbltype", "Mou");
                myStone->setLabel("lblorigine", "Germanie");
                myStone->setLabel("lbltempsechage", "10 secondes");
                myStone->setLabel("lbltemperaturemin", "24 Celcius");
                myStone->setLabel("lblboisfour", "Saule");
                myStone->setLabel("lbltemprestant", "0s/10s");
                myStone->setLabel("lbltemperatureminfour", "(min : 24 Celcius)");
            }
            else if (stoneNomBois == "Chen")
            {
                myStone->setLabel("lblsechage", "");
                boisNom = "Chene";
                std::cout << "Bois Chêne choisi" << std::endl;
                myStone->setLabel("lblbois", "Chêne");
                myStone->setLabel("lbltype", "Dur");
                myStone->setLabel("lblorigine", "USA");
                myStone->setLabel("lbltempsechage", "15 secondes");
                myStone->setLabel("lbltemperaturemin", "24 Celcius");
                myStone->setLabel("lblboisfour", "Chêne");
                myStone->setLabel("lbltemprestant", "0s/15s");
                myStone->setLabel("lbltemperatureminfour", "(min : 24 Celcius)");
            }
            else if (stoneNomBois == "Acajo")
            {
                myStone->setLabel("lblsechage", "");
                boisNom = "Acajou";
                std::cout << "Bois Acajou choisi" << std::endl;
                myStone->setLabel("lblbois", "Acajou");
                myStone->setLabel("lbltype", "Exotique");
                myStone->setLabel("lblorigine", "Brésil");
                myStone->setLabel("lbltempsechage", "18 secondes");
                myStone->setLabel("lbltemperaturemin", "23 Celcius");
                myStone->setLabel("lblboisfour", "Acajou");
                myStone->setLabel("lbltemprestant", "0s/18s");
                myStone->setLabel("lbltemperatureminfour", "(min : 23 Celcius)");
            }
            break;
        }
      }

  if(rd.id<0) std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type<< ")\n";
}


void setup() {
  Serial.begin(9600);

  Serial.println("Stone serial Txd is on pin: "+String(TXD2));
  Serial.println("Stone serial Rxd is on pin: "+String(RXD2));
  myStone = new MyStone(115200, SERIAL_8N1, RXD2, TXD2);


  myTemp->init();


  myButtonT4->init(T4);
  myButtonT4->autoSensibilisation(); //Trouve la sensibilité automatiquement

  myButtonT5->init(T5);
  myButtonT5->autoSensibilisation(); //Trouve la sensibilité automatiquement

  cout << std::string("Début de l'exemple Stone de base pour le ESP32")  << "\n";
}

void loop() {
  temperature = myTemp->getTemp();
  char strTemperature[64];
  sprintf(strTemperature, "%g Celcius", temperature);

  myStone->setLabel("lbltemperature", strTemperature);

  readStoneData();

  if (fourEnCuisson == 1)
  {
      myStone->setLabel("lblsechage", "Temperature trop faible.");
      while (temperature > temperatureMin && temperature < ((temperatureMin * 0.10) + temperatureMin))
      {
          if(compteurTempsSechage < tempsSechage)
          {
                myStone->setLabel("lblsechage", "Cuisson en cours");
                std::string temps = "";
                char strTempsSechage[64];
                sprintf(strTempsSechage, "%d s", tempsSechage);

                compteurTempsSechage++;
                sprintf(strCompteurTempsSechage, "%d /", compteurTempsSechage);
                temps += strCompteurTempsSechage;
                temps += strTempsSechage;
                
                const char * c = temps.c_str();

                myStone->setLabel("lbltemprestant", c);
                delay(1000);
          }
          else
          {
              fourEnCuisson = 0;
              compteurTempsSechage = 0;
              std::cout << "Terminé" << std::endl;
              myStone->setLabel("lblsechage", "Bois sèche !");
              break;
          }
          
      }
      
  }


  int buttonActionT4 = myButtonT4->checkMyButton();
      if(buttonActionT4 > 2)  {  //Si appuyé plus de 0.2 secondes
            Serial.println("Button T4 pressed");
            if(myStone) myStone->changePage();
            }
  int buttonActionT5 = myButtonT5->checkMyButton();
      if(buttonActionT5 > 2)  {  //Si appuyé plus de 0.2 secondes
            Serial.println("Button T5 pressed");
            //Dans la version  1.2, nous allons remplacer ces lignes pour utiliser la
            //méthode getVersion()
            //char cmdFormat2[99];
            //strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_version\",\"type\":\"system\"}>ET");
            //std::cout << cmdFormat2 << "\n";
            //myStone->writeIt((char*)cmdFormat2);
            if(myStone) myStone->getVersion();
          }
  }