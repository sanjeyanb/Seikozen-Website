#include <Wire.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define MQ3_PIN A0
#define BUZZER_PIN D6
#define ALCOHOL_THRESHOLD 45

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 12, "Alcohol Detector");
  u8g2.sendBuffer();
  delay(2000);
  pinMode(MQ3_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  WiFi.begin(ssid, password);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(15, 20, "Connecting WiFi...");
  u8g2.sendBuffer();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 20, "WiFi Connected!");
  u8g2.setCursor(10, 40);
  u8g2.print(WiFi.localIP());
  u8g2.sendBuffer();
  delay(2000);
}

void loop() {
  int analogValue = analogRead(MQ3_PIN);
  int alcoholPercentage = map(analogValue, 0, 1023, 0, 100);
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" | Alcohol Level: ");
  Serial.print(alcoholPercentage);
  Serial.println(" %");
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(15, 12, "Alcohol Level:");
  u8g2.setCursor(15, 28);
  u8g2.print("Concentration: ");
  u8g2.print(alcoholPercentage);
  u8g2.print(" %");
  if (alcoholPercentage > ALCOHOL_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
    static int warningStartX = 128;
    String warningMessage = "WARNING: ALCOHOL LEAK!";
    int warningWidth = warningMessage.length() * 6;
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.setCursor(warningStartX, 48);
    u8g2.print(warningMessage);
    warningStartX -= 2;
    if (warningStartX + warningWidth < 0) {
      warningStartX = 128;
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi OK | Alcohol: ");
    Serial.print(alcoholPercentage);
    Serial.println(" %");
  }
  u8g2.sendBuffer();
  delay(200);
}
