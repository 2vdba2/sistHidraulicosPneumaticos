#include <LiquidCrystal.h>
const int pinoAvanco = 2;
const int pinoRetorno = 3;
const int pinoOutX = 9;
float posX = 0.0;
const float incremento = 0.01;
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
  pinMode(pinoOutX, OUTPUT);      // Saída PWM
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
  pwm=mapearParaPWM(posX, 0.0, 1.0);
  analogWrite(pinoOutX, pwm);  
  lcd.clear();
  lcd.print("X: ");
  lcd.print(posX);
  delay(250);  // espera 2 segundos antes de atualizar
  // Aqui você pode adicionar código para atualizar o display
  Serial.println(digitalRead(pinoAvanco));
}