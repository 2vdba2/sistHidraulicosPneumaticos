#ifndef PRIMARYMOVEMENTS_H
#define PRIMARYMOVEMENTS_H

float converterAnalogParaFloat(int leitura, float xMin, float xMax);
void succao(bool ativo);
void moveZUp();
void printXY();
void moveZDown();
void moveXY(float X, float Y);
void takeAtOrigin();
void leaveAtRed();
void leaveAtGreen();
void leaveAtBlue();
#endif
