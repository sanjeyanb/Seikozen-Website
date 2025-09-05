#include <SPI.h>
#include <MFRC522.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SS_PIN D2
#define RST_PIN D1
#define DHT_PIN D4

#define DHT_TYPE DHT11

const char* ssid = "Redmi Note 12 Pro +5G";
const char* password = "12345677";

const char* serverUrl = "http://192.168.92.208:5000/upload";

const char* medicationCodes[] = {
  "EY210120230001",
  "IN100920230006",
  "LA061220230011",
  "NT200920230018",
  "PK201020230025"
};

MFRC522 mfrc522(SS_PIN, RST_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String barcode = getBarcodeFromRFID(mfrc522.uid);
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Error reading DHT sensor data.");
      return;
    }
    Serial.print("Tag ID: ");
    Serial.println(barcode);
    Serial.print("Temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity);
    sendDataToServer(barcode, temperature, humidity);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
  }
}

String getBarcodeFromRFID(MFRC522::Uid uid) {
  if (uid.size == 4) {
    if (uid.uidByte[0] == 0xF3 && uid.uidByte[1] == 0xE1 && uid.uidByte[2] == 0xB9 && uid.uidByte[3] == 0xF5) {
      return "EY210120230001";
    } else if (uid.uidByte[0] == 0x75 && uid.uidByte[1] == 0xF8 && uid.uidByte[2] == 0x0F && uid.uidByte[3] == 0xAD) {
      return "IN100920230006";
    } else if (uid.uidByte[0] == 0xF5 && uid.uidByte[1] == 0xD3 && uid.uidByte[2] == 0x9E && uid.uidByte[3] == 0xAC) {
      return "LA061220230011";
    } else if (uid.uidByte[0] == 0x5D && uid.uidByte[1] == 0x54 && uid.uidByte[2] == 0x01 && uid.uidByte[3] == 0x32) {
      return "NT200920230018";
    } else if (uid.uidByte[0] == 0x85 && uid.uidByte[1] == 0xFE && uid.uidByte[2] == 0x07 && uid.uidByte[3] == 0xAD) {
      return "PK201020230025";
    }
  }
  return "";
}

void sendDataToServer(String barcode, float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    http.begin(client, serverUrl);
    String payload = "{\"barcode\":\"" + barcode + "\", \"temperature\":" + temperature + ", \"humidity\":" + humidity + "}";
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);
    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Error sending data: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("Error: Not connected to Wi-Fi");
  }
}
