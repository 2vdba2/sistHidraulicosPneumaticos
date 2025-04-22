const int pinoAvancoX = 8;
const int pinoRetornoX = 9;
const int pinoAvancoY = 10;
const int pinoRetornoY = 11;
const int pinoAvancoZ = 12;
const int pinoRetornoZ = 13;
const int pinoReedSwitchZBaixo = 6;
const int pinoReedSwitchZAlto = 7;
float xInput;
float yInput;
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

void setup() {
  pinMode(pinoAvancoX, OUTPUT);
  pinMode(pinoRetornoX, OUTPUT);
  pinMode(pinoAvancoY, OUTPUT);
  pinMode(pinoRetornoY, OUTPUT);
  pinMode(pinoAvancoZ, OUTPUT);
  pinMode(pinoRetornoZ, OUTPUT);
  pinMode(pinoReedSwitchZBaixo, INPUT);
  pinMode(pinoReedSwitchZAlto , INPUT);
  Serial.begin(9600);
}

void loop() {

  //digitalWrite(pinoAvancoX, LOW);  // Desativa o relé 1
  //digitalWrite(pinoRetornoX, HIGH); // Ativa o relé 2
  //delay(1000);
  xInput=converterAnalogParaFloat(analogRead(A0), 0, 1);
  yInput=converterAnalogParaFloat(analogRead(A1), 0, 1);
  Serial.println(xInput);   // Mostra a posição "recebida"
  
//MOVIMENTO X
  if(xInput<x0Placa1-tol){
    digitalWrite(pinoAvancoX, HIGH);  
    digitalWrite(pinoRetornoX, LOW); 
  }
  else if(xInput>x0Placa1+tol){
    digitalWrite(pinoAvancoX, LOW);
    digitalWrite(pinoRetornoX, HIGH); 
  }
  else{
    digitalWrite(pinoAvancoX, LOW);
    digitalWrite(pinoRetornoX, LOW);
  }
//MOVIMENTO Y
   if(yInput<y0Placa1-tol){
    digitalWrite(pinoAvancoY, HIGH);  
    digitalWrite(pinoRetornoY, LOW); 
  }
  else if(yInput>y0Placa1+tol){
    digitalWrite(pinoAvancoY, LOW);
    digitalWrite(pinoRetornoY, HIGH); 
  }
  else{
    digitalWrite(pinoAvancoY, LOW);
    digitalWrite(pinoRetornoY, LOW);
  }
//MOVIMENTO Z
  if(digitalRead(pinoReedSwitchZBaixo)==LOW){
    digitalWrite(pinoAvancoZ, HIGH);  
    digitalWrite(pinoRetornoZ, LOW); 
  }
  //digitalWrite(pinoRetornoZ, HIGH); 
//  else{
//    digitalWrite(pinoAvancoZ, LOW);  
//    digitalWrite(pinoRetornoZ, LOW); 
//    
//  }
//  else if(yInput>y0Placa1+tol){
//    digitalWrite(pinoAvancoY, LOW);
//    digitalWrite(pinoRetornoY, HIGH); 
//  }
//  else{
//    digitalWrite(pinoAvancoY, LOW);
//    digitalWrite(pinoRetornoY, LOW);
//  }
delay(250);
}
