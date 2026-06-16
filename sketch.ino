#include <WiFi.h>
#include <WebSocketsClient.h>
#include <DHT.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* websocket_server = "10.0.2.2";
const uint16_t websocket_port = 8080;
const char* websocket_path = "/";

#define DHTPIN 4
#define DHTTYPE DHT22

#define BOTAO_PORTA 18
#define BOTAO_TESTE 19

#define LED_VERDE 26
#define LED_VERMELHO 27

#define BUZZER 25

DHT dht(DHTPIN, DHTTYPE);
WebSocketsClient webSocket;
bool manualAlert = false;
unsigned long lastSendMillis = 0;

void connectToWiFi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("WiFi conectado: ");
  Serial.println(WiFi.localIP());
}

String buildSensorPayload(float temperatura, const char* porta) {
  String payload = "{";
  payload += "\"source\":\"esp32\",";
  payload += "\"type\":\"sensor\",";
  payload += "\"temperatura\":" + String(temperatura, 1) + ",";
  payload += "\"porta\":\"" + String(porta) + "\",";
  payload += "\"manualAlert\":" + String(manualAlert ? "true" : "false");
  payload += "}";
  return payload;
}

void sendSensorData() {
  float temperatura = dht.readTemperature();
  if (isnan(temperatura)) {
    Serial.println("Falha ao ler temperatura do DHT22");
    return;
  }

  bool portaAberta = digitalRead(BOTAO_PORTA) == LOW;
  const char* porta = portaAberta ? "aberta" : "fechada";
  bool sensorAlert = temperatura > 5.0 || portaAberta;
  bool alerta = sensorAlert || manualAlert;

  digitalWrite(LED_VERDE, alerta ? LOW : HIGH);
  digitalWrite(LED_VERMELHO, alerta ? HIGH : LOW);
  if (alerta) {
    tone(BUZZER, 1000);
  } else {
    noTone(BUZZER);
  }

  String payload = buildSensorPayload(temperatura, porta);
  if (webSocket.isConnected()) {
    webSocket.sendTXT(payload);
    Serial.println("Dados enviados ao servidor:");
    Serial.println(payload);
  }
}

void handleServerCommand(const String& message) {
  if (message.indexOf("\"type\":\"command\"") < 0) {
    return;
  }

  if (message.indexOf("\"command\":\"manualAlert\"") >= 0) {
    manualAlert = message.indexOf("\"value\":true") >= 0;
    Serial.printf("Comando manualAlert recebido: %s\n", manualAlert ? "true" : "false");
  }
}

void webSocketEvent(WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("WebSocket desconectado");
      break;
    case WStype_CONNECTED:
      Serial.println("WebSocket conectado ao servidor");
      webSocket.sendTXT("{\"source\":\"esp32\",\"type\":\"register\",\"client\":\"esp32\"}");
      sendSensorData();
      break;
    case WStype_TEXT:
      handleServerCommand(String((char*)payload));
      break;
    case WStype_ERROR:
      Serial.println("Erro no WebSocket");
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BOTAO_PORTA, INPUT_PULLUP);
  pinMode(BOTAO_TESTE, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  dht.begin();

  Serial.println("CarneCerta IoT iniciado");
  connectToWiFi();

  webSocket.begin(websocket_server, websocket_port, websocket_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  webSocket.enableHeartbeat(15000, 30000, 2);
}

void loop() {
  webSocket.loop();

  if (millis() - lastSendMillis > 2000) {
    lastSendMillis = millis();
    sendSensorData();
  }
}
