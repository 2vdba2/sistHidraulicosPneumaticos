#include <Arduino.h>
#include "primaryMovements.h"
#include "globals.h"
#include "emergencyStop.h"

// Inicializa os componentes da interface (botões, LEDs, buzzer, etc.)
void printMenu(){
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
}

void interfaceRobo(){
 if (esperandoComando) {
    //TIMSK1 &= ~(1 << OCIE1A);  // Desativa interrupção se necessário
    emergencyStop = false;
    stop();
    printMenu();
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
