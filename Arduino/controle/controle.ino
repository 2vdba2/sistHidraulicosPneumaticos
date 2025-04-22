const int pinoAvancoX = 8;
const int pinoRetornoX = 9;
const int pinoAvancoY = 10;
const int pinoRetornoY = 11;
const int pinoAvancoZ = 12;
const int pinoRetornoZ = 13;
const int pinoReedSwitchZBaixo = 6;
const int pinoReedSwitchZAlto = 7;
const int pinoSuccao = 5;

float x0Placa1=0.2;
float y0Placa1=0.4;
float xRed=0.4;
float yRed=0.6;
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
void moveXY(float X, float Y){
  float xInput;
  float yInput;
  bool xOK = false;
  bool yOK = false;
  bool zHigh = false;
  bool zLow = false;
  //Subir Z
  while(!digitalRead(pinoReedSwitchZAlto)){
    digitalWrite(pinoAvancoZ, HIGH);  
    digitalWrite(pinoRetornoZ, LOW);
    delay(250);    
  }
  digitalWrite(pinoAvancoZ, LOW);  
  digitalWrite(pinoRetornoZ, LOW);
  //Enquanto x e y nao estiverem na posicao desejada ficar preso nesse loop
  while(!(xOK && xOK)){
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
  while(!digitalRead(pinoReedSwitchZBaixo)){
    digitalWrite(pinoAvancoZ, LOW);  
    digitalWrite(pinoRetornoZ, HIGH);
    delay(250);    
  }
  digitalWrite(pinoAvancoZ, LOW);  
  digitalWrite(pinoRetornoZ, LOW);
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
  moveXY(0.3 ,0.4);
  succao(true);
  delay(10000);
}
