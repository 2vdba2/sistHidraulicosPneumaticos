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

const float tol = 0.02;
char comando;
char esperandoComando = 1;
volatile bool emergencyStop = false;
// Configura o Timer1 para gerar interrupção a cada 1s (aprox.)
void setupTimer1() {
  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 1 Hz com prescaler de 1024: OCR1A = 16e6 / (1024 * 1) - 1
  OCR1A = 15624;
  TCCR1B |= (1 << WGM12);  // Modo CTC
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler 1024
  TIMSK1 |= (1 << OCIE1A);  // Ativa interrupção de comparação

  sei(); // Reativa interrupções
}
void stop() {
  digitalWrite(pinoSuccao, LOW);
  digitalWrite(pinoAvancoX, LOW);
  digitalWrite(pinoRetornoX, LOW);
  digitalWrite(pinoAvancoY, LOW);
  digitalWrite(pinoRetornoY, LOW);
  digitalWrite(pinoAvancoZ, LOW);
  digitalWrite(pinoRetornoZ, LOW);
  //Serial.write("\nStop\n");
  //asm volatile ("  jmp 0");//REINICIA MAQUINA
  //esperandoComando=1;
  //emergency;
}

void emergencyStopFunction() {
  emergencyStop = true;
  stop();
  Serial.write("\n----------------\n");
  Serial.write("\nEmergency Stop\n");
  Serial.write("\n----------------\n");
  esperandoComando = 1;
}

ISR(TIMER1_COMPA_vect) {
  if (Serial.available() == 1 && Serial.read() == '0') {
    // Executa algo se último comando foi '0'
    emergencyStopFunction();
    //delay(2000);

  }
}

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
  }
  digitalWrite(pinoAvancoZ, LOW);
  digitalWrite(pinoRetornoZ, LOW);
}

void moveXY(float X, float Y) {

  bool xOK = false;
  bool yOK = false;


  //Subir Z
  moveZUp();

  //Enquanto x e y nao estiverem na posicao desejada ficar preso nesse loop
  while (!(xOK && yOK) && (!emergencyStop)) {
    xInput = converterAnalogParaFloat(analogRead(A0), 0, 1);
    yInput = converterAnalogParaFloat(analogRead(A1), 0, 1);
    //MOVIMENTO X
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
  //MOVIMENTO Z
  moveZDown();
  if (!emergencyStop) {
    printXY();
    /*
      Serial.print("X: ");
      Serial.print(xInput);
      Serial.print(" | Y: ");
      Serial.println(yInput);*/
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
  setupTimer1();

}

void loop() {

  if (esperandoComando) {
    //TIMSK1 &= ~(1 << OCIE1A);  // Desativa interrupção se necessário
    emergencyStop = false;
    stop();
    Serial.write("-------------------------------------------------------\n");
    Serial.write("Robo Cartesiano para posicionamentos de pecas por Cor\n");
    Serial.write("Escolha Acao:\n");
    Serial.write("0: Emergency Stop \n");
    Serial.write("1: Start \n");
    Serial.write("2: Go to ORIGIN \n");
    Serial.write("3: Go to RED \n");
    Serial.write("4: Go to BLUE \n");
    Serial.write("5: Go to GREEN \n");
    Serial.write("6: Pulse Suction \n");
    Serial.write("7: Print XY Position \n");
    //Serial.write("8:  \n");
    Serial.write("x: moveXY(1,0) \n");
    Serial.write("y: moveXY(0,1) \n");
    Serial.write("z: moveZUp() \n");
    Serial.write("-------------------------------------------------------\n");
    esperandoComando = 0;
  }

  // Verifica se há dados na serial
  if (Serial.available()) {
    //sei();
    //TIMSK1 |= (1 << OCIE1A);  // Ativa interrupção
    emergencyStop = false;
    esperandoComando = 1;


    comando = Serial.read();  // Lê o próximo caractere da serial
    //Serial.write(comando);

    // Se o caractere for um dígito de 0 a 5, processa o comando

    switch (comando) {  // Usamos o caractere diretamente no switch
      case '0':  // Start
        stop();
        Serial.write("\n----------------\n");
        Serial.write("\nEmergency Stop\n");
        Serial.write("\n----------------\n");
        break;
      case '1':  // Start
        Serial.println("Comando 1 acionado");

        takeAtOrigin();
        leaveAtRed();
        break;
      case '2':  // Red
        Serial.println("Comando 2 acionado");
        moveXY(xRed, yRed);
        break;
      case '3':  // Green
        Serial.println("Comando 3 acionado");
        moveXY(xGreen, yGreen);
        break;
      case '4':  // Blue
        Serial.println("Comando 4 acionado");
        moveXY(xBlue, yBlue);
        break;
      case '5':  // Origin
        Serial.println("Comando 5 acionado");
        moveXY(xUndef , yUndef);
        break;
      case '6':  // Origin
        Serial.println("Comando 6 acionado");
        succao(true);
        break;
      case '7':  // Origin
        Serial.println("Comando 7 acionado");
        printXY();
        break;
      /*
        case '8':  // Origin
        Serial.println("Comando 8 acionado");
        printXY();
        break;
      */
      case 'x':  // Origin
        Serial.println("Comando x acionado");
        moveXY(1.0, 0.0);
        break;
      case 'y':  // Origin
        Serial.println("Comando y acionado");
        moveXY(0.0, 1.0);
        break;
      case 'z':  // Origin
        Serial.println("Comando z acionado");
        moveZUp();
        break;
      default:  // Comando inválido
        Serial.println("Comando inválido");
        break;
    }


  }

  //takeAtOrigin();
  //leaveAtRed();
  delay(250);
}
