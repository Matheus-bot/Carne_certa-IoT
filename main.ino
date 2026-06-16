#include <WiFi.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando o CarneCerta IoT...");
  
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi simulado");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConectado com sucesso!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(1000);
}