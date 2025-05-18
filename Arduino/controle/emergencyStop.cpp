#include <Arduino.h>
#include "primaryMovements.h"
#include "globals.h"
#include "emergencyStop.h"

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

void testEmergency(void){
  if (Serial.available() == 1 && Serial.read() == '0') {
      emergencyStopFunction();
  }
}
