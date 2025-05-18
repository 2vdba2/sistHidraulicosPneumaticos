
#include <Arduino.h>
#include "primaryMovements.h"
#include "globals.h"
#include "emergencyStop.h"
#include "interface.h"



void setup() {

  pinMode(pinoAvancoX, OUTPUT);
  pinMode(pinoRetornoX, OUTPUT);
  pinMode(pinoAvancoY, OUTPUT);
  pinMode(pinoRetornoY, OUTPUT);
  pinMode(pinoAvancoZ, OUTPUT);
  pinMode(pinoRetornoZ, OUTPUT);
  pinMode(pinoSuccao, OUTPUT);
  pinMode(pinoReedSwitchZBaixo, INPUT);
  pinMode(pinoReedSwitchZAlto , INPUT);
  Serial.begin(9600);
  setupTimer1();

}

void loop() {
  interfaceRobo();
}
