#define  pinoSuccao 4
#define  pinoReedSwitchZBaixo 5
#define  pinoReedSwitchZAlto 6
#define  pinoAvancoX 8
#define  pinoRetornoX 9
#define  pinoAvancoY 10
#define  pinoRetornoY 11
#define  pinoAvancoZ 12
#define  pinoRetornoZ 13
//#include <TimerInterrupt.h>
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
char comando;
char esperandoComando=1;
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
  //Timer1.initialize(interval * 1000); // O valor é em microssegundos
  //Timer1.attachInterrupt(checkSerial);
}

void loop() {
  if(esperandoComando){
    Serial.write("-------------------------------------------------------\n");
    Serial.write("Robo Cartesiano para posicionamentos de pecas por Cor\n");
    Serial.write("Escolha Acao:\n");
    Serial.write("0: Emergency Stop \n");
    Serial.write("1: Start \n");
    Serial.write("2: Go to ORIGIN \n");
    Serial.write("3: Go to RED \n");
    Serial.write("4: Go to BLUE \n");
    Serial.write("5: Go to GREEN \n");
    Serial.write("6: Activate Suction \n");
    Serial.write("7: Deactivate Suction \n");
    Serial.write("-------------------------------------------------------\n");
    esperandoComando=0;
  }

  // Verifica se há dados na serial
  if (Serial.available()) {
    esperandoComando=1;

    
    comando = Serial.read();  // Lê o próximo caractere da serial
    //Serial.write(comando);

    // Se o caractere for um dígito de 0 a 5, processa o comando
    
      switch (comando) {  // Usamos o caractere diretamente no switch
        case '1':  // Start
          Serial.println("Comando 1 acionado");
          
          takeAtOrigin();
          leaveAtRed();
          break;
        case '2':  // Red
          Serial.println("Comando 2 acionado");
          break;
        case '3':  // Green
          Serial.println("Comando 3 acionado");
          break;
        case '4':  // Blue
          Serial.println("Comando 4 acionado");
          break;
        case '5':  // Origin
          Serial.println("Comando 5 acionado");
          break;
        default:  // Comando inválido
          Serial.println("Comando inválido");
          break;
      }
    

  }  

  //takeAtOrigin();
  //leaveAtRed();
}
