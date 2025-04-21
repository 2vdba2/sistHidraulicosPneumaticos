const int avancoX = 8;
const int retornoX = 9;
int valorRecebido;
float converterAnalogParaFloat(int leitura, float xMin, float xMax) {
  if (xMin == xMax) return xMin; // evita divisão por zero

  // Garante que a leitura está dentro do intervalo esperado
  if (leitura < 0) leitura = 0;
  if (leitura > 1023) leitura = 1023;

  float proporcao = leitura / 1023.0;
  return xMin + proporcao * (xMax - xMin);
}

void setup() {
  pinMode(avancoX, OUTPUT);
  pinMode(retornoX, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(avancoX, HIGH); // Ativa o relé 1
  digitalWrite(retornoX, LOW);  // Desativa o relé 2

  //digitalWrite(avancoX, LOW);  // Desativa o relé 1
  //digitalWrite(retornoX, HIGH); // Ativa o relé 2
  //delay(1000);
  valorRecebido = analogRead(A0);  // Lê sinal vindo do filtro RC
  Serial.println(converterAnalogParaFloat(valorRecebido, 0, 1));   // Mostra a posição "recebida"
  delay(250);
}
