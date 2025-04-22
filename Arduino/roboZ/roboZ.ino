#include <LiquidCrystal.h>
const int pinoAvanco = 2;
const int pinoRetorno = 3;
const int pinoReedSwitchZBaixo = 9;
const int pinoReedSwitchZAlto = 10;
float posX = 0.9;
int reedSwitchZBaixo=0;
int reedSwitchZAlto=0;
const float incremento = 0.01;
const float tol=0.02;
// Inicializa o LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 4, 5, 6, 7);
// Função que converte posição X para PWM
int mapearParaPWM(float x, float xMin, float xMax) {
  if (xMin == xMax) return 0;

  if (x < xMin) x = xMin;
  if (x > xMax) x = xMax;

  float pwm = (x - xMin) / (xMax - xMin) * 255.0;
  return (int)pwm;
}
void setup() {
  pinMode(pinoAvanco, INPUT);
  pinMode(pinoRetorno, INPUT);
  pinMode(pinoReedSwitchZBaixo, OUTPUT);
  pinMode(pinoReedSwitchZAlto, OUTPUT);        
  lcd.begin(16, 2);    // Inicia o LCD com 16 colunas e 2 linhas
  //lcd.print("Olá, Mundo!");  // Exibe a mensagem "Olá, Mundo!"
  Serial.begin(9600);
}

void loop() {
  int pwm;
  if (digitalRead(pinoAvanco) == HIGH) {
    posX += incremento;
  } else if (digitalRead(pinoRetorno) == HIGH) {
    posX -= incremento;
  }
//MOVIMENTO X
  if(posX<tol){
    digitalWrite(pinoReedSwitchZBaixo, HIGH);
    reedSwitchZBaixo=1; 
  }
  else if(posX>1-tol){
    digitalWrite(pinoReedSwitchZAlto, HIGH);
    reedSwitchZAlto=1; 
  }
  else{
    digitalWrite(pinoReedSwitchZAlto, LOW);
    digitalWrite(pinoReedSwitchZBaixo, LOW);
    reedSwitchZBaixo=0;
    reedSwitchZAlto=0; 
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Z: ");
  lcd.print(posX);
  lcd.setCursor(0, 1);
  lcd.print("Zb: ");
  lcd.print(reedSwitchZBaixo);
  lcd.print("; Za: ");
  lcd.print(reedSwitchZBaixo);
  Serial.println(digitalRead(pinoReedSwitchZBaixo));
  delay(250);  // espera 2 segundos antes de atualizar
  // Aqui você pode adicionar código para atualizar o display
  
}
