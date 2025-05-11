#ifndef GLOBALS_H
#define GLOBALS_H

#define  pinoSuccao 4
#define  pinoReedSwitchZBaixo 5
#define  pinoReedSwitchZAlto 6
#define  pinoAvancoX 8
#define  pinoRetornoX 9
#define  pinoAvancoY 10
#define  pinoRetornoY 11
#define  pinoAvancoZ 12
#define  pinoRetornoZ 13

//--------------------------------
//Mapeamento
//--------------------------------
//Limites
extern float x0;
extern float y0;
extern float xf;
extern float yf;
//Posicoes
//RED GREEN BLUE
//min 300 max 700
//700---------------------
//   ---RED--GREEN--BLUE--
//   ---------------------
//   ---------(X)---------
//300---------------------
//   |                   |
// 300                  700
extern float xInput;
extern float yInput;

extern float xRed;
extern float xGreen;
extern float xBlue;
extern float xUndef;

extern float yRed;
extern float yGreen;
extern float yBlue;
extern float yUndef;

extern const float tol;
extern char comando;
extern char esperandoComando;
extern volatile bool emergencyStop;

#endif
