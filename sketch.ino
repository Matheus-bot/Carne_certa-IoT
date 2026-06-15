#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

#define BOTAO_PORTA 18
#define BOTAO_TESTE 19

#define LED_VERDE 26
#define LED_VERMELHO 27

#define BUZZER 25

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);

  pinMode(BOTAO_PORTA, INPUT_PULLUP);
  pinMode(BOTAO_TESTE, INPUT_PULLUP);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  dht.begin();

  Serial.println("CarneCerta IoT iniciado");
}

void loop() {

  float temperatura = dht.readTemperature();

  bool portaAberta = digitalRead(BOTAO_PORTA) == LOW;

  bool testeManual = digitalRead(BOTAO_TESTE) == LOW;

  bool alerta = false;

  if (temperatura > 5) {
    alerta = true;
  }

  if (portaAberta) {
    alerta = true;
  }

  if (testeManual) {
    alerta = true;
  }

  if (alerta) {

    digitalWrite(LED_VERDE, LOW);

    digitalWrite(LED_VERMELHO, HIGH);

    tone(BUZZER, 1000);

  } else {

    digitalWrite(LED_VERDE, HIGH);

    digitalWrite(LED_VERMELHO, LOW);

    noTone(BUZZER);
  }

  Serial.println("==============");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");

  Serial.print("Porta: ");

  if (portaAberta) {
    Serial.println("ABERTA");
  } else {
    Serial.println("FECHADA");
  }

  Serial.print("Teste Manual: ");

  if (testeManual) {
    Serial.println("ATIVADO");
  } else {
    Serial.println("DESATIVADO");
  }

  delay(1000);
}
