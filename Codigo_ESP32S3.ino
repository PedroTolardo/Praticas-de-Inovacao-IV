#include <WiFi.h>
#include <WebServer.h>

// Substitua pelos pinos disponíveis e confiáveis para sua placa ESP32-S3
#define LED_VERDE_MARTELO    10
#define LED_VERMELHO_MARTELO 11
#define LED_VERDE_ALICATE    12
#define LED_VERMELHO_ALICATE 13

const char* ssid = "FloNa";
const char* password = "270324fn";

WebServer server(80);

unsigned long tempoUltimoEvento = 0;
bool objetoDetectado = false;

void manterEstadoInicial() {
  digitalWrite(LED_VERDE_MARTELO, LOW);
  digitalWrite(LED_VERDE_ALICATE, LOW);
  digitalWrite(LED_VERMELHO_MARTELO, HIGH);
  digitalWrite(LED_VERMELHO_ALICATE, HIGH);
  objetoDetectado = false;
}

void handlePost() {
  String body = server.arg("plain");
  Serial.println("Recebido: " + body);

  manterEstadoInicial(); // Resetar antes de ativar o novo estado

  if (body == "Martelo") {
    Serial.println("Comando: martelo");
    digitalWrite(LED_VERMELHO_MARTELO, LOW);
    digitalWrite(LED_VERDE_MARTELO, HIGH);
    objetoDetectado = true;
    tempoUltimoEvento = millis();
  } else if (body == "Alicate") {
    Serial.println("Comando: Alicate");
    digitalWrite(LED_VERMELHO_ALICATE, LOW);
    digitalWrite(LED_VERDE_ALICATE, HIGH);
    objetoDetectado = true;
    tempoUltimoEvento = millis();
  }

  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_VERDE_MARTELO, OUTPUT);
  pinMode(LED_VERMELHO_MARTELO, OUTPUT);
  pinMode(LED_VERDE_ALICATE, OUTPUT);
  pinMode(LED_VERMELHO_ALICATE, OUTPUT);

  manterEstadoInicial();

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado. IP: " + WiFi.localIP().toString());

  server.on("/", HTTP_POST, handlePost);
  server.begin();
}

void loop() {
  server.handleClient();

  if (objetoDetectado && millis() - tempoUltimoEvento > 5000) {
    manterEstadoInicial();
    Serial.println("Tempo expirado, resetando LEDs.");
  }
}
