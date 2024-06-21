#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI   13
#define OLED_CLK   14
#define OLED_DC    5
#define OLED_CS    15
#define OLED_RESET 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define pino_sinal_analogico A0

// Nome da rede e senha
char ssid[] = "projetos_auto"; // Nome da rede SSID
char pass[] = "CybertronIFM23"; // Nome da senha

int status = WL_IDLE_STATUS;
WiFiClient client;
int sensorValue; // variável que armazena o valor analógico lido

// Informações disponíveis no ThingSpeak, número de canal e chave de escrita
unsigned long canal = 1726351;
const char * ChaveEscritaAPI = "63TPBDTTMB8WEWHD"; // Substitua pela sua chave de escrita

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
  
  ThingSpeak.begin(client);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Iniciar display OLED
  {
    Serial.println(F("Falha na inicialização do display SSD1306"));
    for(;;);
  }
  
  display.clearDisplay();
  display.display();
  delay(100);
}

void loop() {
  // Lê o valor da entrada no pino analógico 0 (LDR conectado ao A0)
  sensorValue = analogRead(pino_sinal_analogico);
  
  // Escreve o valor lido no Serial Monitor
  Serial.print("Leitura: ");
  Serial.println(sensorValue);

  // Escreve no ThingSpeak:
  // (número do canal, número do campo, valor, chave de escrita)
  int resposta = ThingSpeak.writeField(canal, 1, sensorValue, ChaveEscritaAPI);
  
  if (resposta == 200) {
    Serial.println("Dados enviados com sucesso ao ThingSpeak");
  } else {
    Serial.println("Erro ao enviar dados ao ThingSpeak");
  }

  exibeValorSensor(sensorValue);
  
  delay(20000); // ThingSpeak aceita atualizações a cada 15 segundos (20 segundos para garantir).
}

void exibeValorSensor(int valor) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Valor do Sensor:");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(valor);
  display.display();
  display.invertDisplay(true);
  delay(2000);
  display.invertDisplay(false);
  delay(2000); // Manter o valor exibido por 2 segundos
}
