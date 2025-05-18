#include <Arduino.h>
#include "globals.h"
//--------------------------------
//Mapeamento
//--------------------------------
//Limites
float x0 = 300.0 / 1024;
float y0 = 300.0 / 1024;
float xf = 700.0 / 1024;
float yf = 700.0 / 1024;
//Posicoes
//RED GREEN BLUE
//min 300 max 700
//700---------------------
//   ---RED--GREEN--BLUE--
//   ---------------------
//   ---------(X)---------
//300---------------------
//   |                   |
// 300
float xInput;
float yInput;

float xRed  = 1 * ((xf - x0) / 6) + x0;
float xGreen = 3 * ((xf - x0) / 6) + x0;
float xBlue = 5 * ((xf - x0) / 6) + x0;
float xUndef = 3 * ((xf - x0) / 6) + x0;

float yRed  = 3 * ((yf - y0) / 4) + y0;
float yGreen = 3 * ((yf - y0) / 4) + y0;
float yBlue = 3 * ((yf - y0) / 4) + y0;
float yUndef = 1 * ((yf - y0) / 4) + y0;

const float tol = 0.05;
char comando;
char esperandoComando = 1;
volatile bool emergencyStop = false;
