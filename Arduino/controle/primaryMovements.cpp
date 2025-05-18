#include <Arduino.h>
#include "primaryMovements.h"
#include "globals.h"
#include "emergencyStop.h"
float converterAnalogParaFloat(int leitura, float xMin, float xMax) {
  if (xMin == xMax) return xMin; // evita divisão por zero

  // Garante que a leitura está dentro do intervalo esperado
  if (leitura < 0) leitura = 0;
  if (leitura > 1023) leitura = 1023;

  float proporcao = leitura / 1023.0;
  return xMin + proporcao * (xMax - xMin);
}
void succao(bool ativo) {
  if (ativo) {
    digitalWrite(pinoSuccao, HIGH);
  }
  else {
    digitalWrite(pinoSuccao, LOW);
  }
  delay(1000);
}

void moveZUp() {
  //Subir Z
  while (!digitalRead(pinoReedSwitchZAlto) && (!emergencyStop)) {
    digitalWrite(pinoAvancoZ, HIGH);
    digitalWrite(pinoRetornoZ, LOW);
    delay(250);
    testEmergency();
  }
  digitalWrite(pinoAvancoZ, LOW);
  digitalWrite(pinoRetornoZ, LOW);
}

void printXY() {
  xInput = converterAnalogParaFloat(analogRead(A0), 0, 1);
  yInput = converterAnalogParaFloat(analogRead(A1), 0, 1);
  Serial.print("X: ");
  Serial.print(xInput);
  Serial.print(" | Y: ");
  Serial.print(yInput);
  Serial.print(" | Zb: ");
  Serial.print(digitalRead(pinoReedSwitchZBaixo));
  Serial.print(" | Za: ");
  Serial.println(digitalRead(pinoReedSwitchZAlto));
}
void moveZDown() {
  while (!digitalRead(pinoReedSwitchZBaixo) && (!emergencyStop)) {
    digitalWrite(pinoAvancoZ, LOW);
    digitalWrite(pinoRetornoZ, HIGH);
    delay(250);
    testEmergency();
  }
  digitalWrite(pinoAvancoZ, LOW);
  digitalWrite(pinoRetornoZ, LOW);
}

void moveX(float X){
  bool xOK = false;
  while (!(xOK) && (!emergencyStop)) {
    xInput = converterAnalogParaFloat(analogRead(A0), 0, 1);
    
    if (xInput < X - tol) {
      digitalWrite(pinoAvancoX, HIGH);
      digitalWrite(pinoRetornoX, LOW);
    }
    else if (xInput > X + tol) {
      digitalWrite(pinoAvancoX, LOW);
      digitalWrite(pinoRetornoX, HIGH);
    }
    else {
      digitalWrite(pinoAvancoX, LOW);
      digitalWrite(pinoRetornoX, LOW);
      xOK = true;
    }
    delay(250);
  }
}

void moveY(float Y){
  
  bool yOK = false;
  while (!(yOK) && (!emergencyStop)) {
    yInput = converterAnalogParaFloat(analogRead(A1), 0, 1);  
    //MOVIMENTO Y
    if (yInput < Y - tol) {
      digitalWrite(pinoAvancoY, HIGH);
      digitalWrite(pinoRetornoY, LOW);
    }
    else if (yInput > Y + tol) {
      digitalWrite(pinoAvancoY, LOW);
      digitalWrite(pinoRetornoY, HIGH);
    }
    else {
      digitalWrite(pinoAvancoY, LOW);
      digitalWrite(pinoRetornoY, LOW);
      yOK = true;
    }
    delay(250);
  }
}

void moveXY(float X, float Y) {
  moveZUp();
  moveX(X);
  moveY(Y);
  moveZDown();
  if (!emergencyStop) {
    printXY();
  }
}

void takeAtOrigin() {
  moveXY(xUndef , yUndef);

  if (!emergencyStop) {
    delay(2000);
    succao(true);
    delay(2000);
    moveZUp();
  }

}

void leaveAtRed() {
  moveXY(xRed, yRed);
  if (!emergencyStop) {
    delay(2000);
    succao(false);
    delay(2000);
    moveZUp();
  }
}
void leaveAtGreen() {
  moveXY(xGreen, yGreen);
  if (!emergencyStop) {
    delay(2000);
    succao(false);
    delay(2000);
    moveZUp();
  }
}
void leaveAtBlue() {
  moveXY(xBlue, yBlue);
  if (!emergencyStop) {
    delay(2000);
    succao(false);
    delay(2000);
    moveZUp();
  }
}
