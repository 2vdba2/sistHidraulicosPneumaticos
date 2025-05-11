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
float x0=300.0/1024;
float y0=300.0/1024;
float xf=700.0/1024;
float yf=700.0/1024;
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
float xRed  =1*((xf-x0)/6)+x0;
float xGreen=3*((xf-x0)/6)+x0;
float xBlue =5*((xf-x0)/6)+x0;
float xUndef=3*((xf-x0)/6)+x0;

float yRed  =3*((yf-y0)/4)+y0;
float yGreen=3*((yf-y0)/4)+y0;
float yBlue =3*((yf-y0)/4)+y0;
float yUndef=1*((yf-y0)/4)+y0;

const float tol=0.02;
float converterAnalogParaFloat(int leitura, float xMin, float xMax) {
  if (xMin == xMax) return xMin; // evita divisão por zero

  // Garante que a leitura está dentro do intervalo esperado
  if (leitura < 0) leitura = 0;
  if (leitura > 1023) leitura = 1023;

  float proporcao = leitura / 1023.0;
  return xMin + proporcao * (xMax - xMin);
}
void succao(bool ativo){
  if(ativo){
    digitalWrite(pinoSuccao, HIGH);
  }
  else{
    digitalWrite(pinoSuccao, LOW);
  }
  delay(1000); 
}

void moveZUp(){
  //Subir Z
  while(!digitalRead(pinoReedSwitchZAlto)){
    digitalWrite(pinoAvancoZ, HIGH);  
    digitalWrite(pinoRetornoZ, LOW);
    delay(250);    
  }
  digitalWrite(pinoAvancoZ, LOW);  
  digitalWrite(pinoRetornoZ, LOW);
}
void moveZDown(){
    while(!digitalRead(pinoReedSwitchZBaixo)){
    digitalWrite(pinoAvancoZ, LOW);  
    digitalWrite(pinoRetornoZ, HIGH);
    delay(250);    
  }
  digitalWrite(pinoAvancoZ, LOW);  
  digitalWrite(pinoRetornoZ, LOW);
}

void moveXY(float X, float Y){
  float xInput;
  float yInput;
  bool xOK = false;
  bool yOK = false;
  //Subir Z
  moveZUp();

  //Enquanto x e y nao estiverem na posicao desejada ficar preso nesse loop
  while(!(xOK && yOK)){
  xInput=converterAnalogParaFloat(analogRead(A0), 0, 1);
  yInput=converterAnalogParaFloat(analogRead(A1), 0, 1);
//MOVIMENTO X
  if(xInput<X-tol){
    digitalWrite(pinoAvancoX, HIGH);  
    digitalWrite(pinoRetornoX, LOW); 
  }
  else if(xInput>X+tol){
    digitalWrite(pinoAvancoX, LOW);
    digitalWrite(pinoRetornoX, HIGH); 
  }
  else{
    digitalWrite(pinoAvancoX, LOW);
    digitalWrite(pinoRetornoX, LOW);
    xOK=true;
  }
//MOVIMENTO Y
   if(yInput<Y-tol){
    digitalWrite(pinoAvancoY, HIGH);  
    digitalWrite(pinoRetornoY, LOW); 
  }
  else if(yInput>Y+tol){
    digitalWrite(pinoAvancoY, LOW);
    digitalWrite(pinoRetornoY, HIGH); 
  }
  else{
    digitalWrite(pinoAvancoY, LOW);
    digitalWrite(pinoRetornoY, LOW);
    yOK=true;
  }
    delay(250);
  }
//MOVIMENTO Z
moveZDown();
  Serial.print("X: ");
  Serial.print(xInput);
  Serial.print(" | Y: ");
  Serial.println(yInput);

}

void takeAtOrigin(){
  moveXY(xUndef ,yUndef);
  delay(2000);
  succao(true);
  delay(2000);
  moveZUp();
}
                
void leaveAtRed(){
  moveXY(xRed,yRed);
  delay(2000);
  succao(false);
  delay(2000);
  moveZUp();
}
void leaveAtGreen(){
  moveXY(xGreen,yGreen);
}
void leaveAtBlue(){
  moveXY(xBlue,yBlue);
}

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
}

void loop() {

  Serial.print("X: ");
  //Serial.print(xInput);
  Serial.print(" | Y: ");
  //Serial.println(yInput);

  takeAtOrigin();
  leaveAtRed();
}
