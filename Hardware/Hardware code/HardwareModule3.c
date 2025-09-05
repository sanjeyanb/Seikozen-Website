#include <SPI.h>
#include <MFRC522.h>
#include <DHT.h>
#include <SoftwareSerial.h>

#define SS_PIN 10
#define RST_PIN 9
#define DHT_PIN 2
#define DHT_TYPE DHT11

#define EYE_DROPS_TEMP_MIN 15
#define EYE_DROPS_TEMP_MAX 25
#define INHALERS_TEMP_MIN 15
#define INHALERS_TEMP_MAX 25
#define LIQUID_ANTIBIOTICS_TEMP_MIN 20
#define LIQUID_ANTIBIOTICS_TEMP_MAX 25
#define NITROGLYCERIN_TABLETS_TEMP_MIN 15
#define NITROGLYCERIN_TABLETS_TEMP_MAX 20
#define PAIN_KILLERS_TEMP_MIN 25
#define PAIN_KILLERS_TEMP_MAX 40

MFRC522 mfrc522(SS_PIN, RST_PIN);
DHT dht(DHT_PIN, DHT_TYPE);

SoftwareSerial gsm(7, 8);

const char* medicationCodes[] = {
  "EY210120230001",
  "IN100920230006",
  "LA061220230011",
  "NT200920230018",
  "PK201020230025"
};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  dht.begin();
  gsm.begin(9600);
  delay(1000);
  gsm.println("AT+CMGF=1");
  delay(1000);
  Serial.println("RFID Tag ID Scanner");
}

void loop() {
  if (SMSRequest()) {
    if (readData()) {
      String dataMessage = "Temperature: " + String(dht.readTemperature()) + "*C Humidity: " + String(dht.readHumidity()) + "%";
      sendSMS("+917448554321", dataMessage);
    }
  }
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("Tag ID: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      Serial.print(" ");
    }
    Serial.print("= ");
    if (mfrc522.uid.uidByte[0] == 0xF3 && mfrc522.uid.uidByte[1] == 0xE1 && mfrc522.uid.uidByte[2] == 0xB9 && mfrc522.uid.uidByte[3] == 0xF5) {
      Serial.print(medicationCodes[0]);
      Serial.println();
      displayLiveData("Eye Drops");
    } else if (mfrc522.uid.uidByte[0] == 0x75 && mfrc522.uid.uidByte[1] == 0xF8 && mfrc522.uid.uidByte[2] == 0x0F && mfrc522.uid.uidByte[3] == 0xAD) {
      Serial.print(medicationCodes[1]);
      Serial.println();
      displayLiveData("Inhalers");
    } else if (mfrc522.uid.uidByte[0] == 0xF5 && mfrc522.uid.uidByte[1] == 0xD3 && mfrc522.uid.uidByte[2] == 0x9E && mfrc522.uid.uidByte[3] == 0xAC) {
      Serial.print(medicationCodes[2]);
      Serial.println();
      displayLiveData("Liquid Antibiotics");
    } else if (mfrc522.uid.uidByte[0] == 0x5D && mfrc522.uid.uidByte[1] == 0x54 && mfrc522.uid.uidByte[2] == 0x01 && mfrc522.uid.uidByte[3] == 0x32) {
      Serial.print(medicationCodes[3]);
      Serial.println();
      displayLiveData("Nitroglycerin Tablets");
    } else if (mfrc522.uid.uidByte[0] == 0x85 && mfrc522.uid.uidByte[1] == 0xFE && mfrc522.uid.uidByte[2] == 0x07 && mfrc522.uid.uidByte[3] == 0xAD) {
      Serial.print(medicationCodes[4]);
      Serial.println();
      displayLiveData("Pain Killers");
    }
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
  }
}

void displayLiveData(String medication) {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  bool alert = false;
  String message = "ALERT: Temperature for " + medication + " is out of range! \n The Temperature is " + temperature + "oc \n Humidity " + humidity +" % Click Link to verify wXXXXX" ;
  if (medication == "Eye Drops" && (temperature < EYE_DROPS_TEMP_MIN || temperature > EYE_DROPS_TEMP_MAX)) {
    alert = true;
  } else if (medication == "Inhalers" && (temperature < INHALERS_TEMP_MIN || temperature > INHALERS_TEMP_MAX)) {
    alert = true;
  } else if (medication == "Liquid Antibiotics" && (temperature < LIQUID_ANTIBIOTICS_TEMP_MIN || temperature > LIQUID_ANTIBIOTICS_TEMP_MAX)) {
    alert = true;
  } else if (medication == "Nitroglycerin Tablets" && (temperature < NITROGLYCERIN_TABLETS_TEMP_MIN || temperature > NITROGLYCERIN_TABLETS_TEMP_MAX)) {
    alert = true;
  } else if (medication == "Pain Killers" && (temperature < PAIN_KILLERS_TEMP_MIN || temperature > PAIN_KILLERS_TEMP_MAX)) {
    alert = true;
  }
  if (alert) {
    Serial.println(message);
    sendSMS("+917448554321", message);
  }
}

void sendSMS(String phoneNumber, String message) {
  gsm.print("AT+CMGS=\"");
  gsm.print(phoneNumber);
  gsm.println("\"");
  delay(1000);
  gsm.println(message);
  delay(1000);
  gsm.write(26);
  delay(1000);
  Serial.println("SMS sent successfully");
}

boolean SMSRequest() {
  if (gsm.available() > 0) {
    String request = gsm.readString();
    if (request.indexOf("STATE") != -1) {
      Serial.println("SMS request detected.");
      return true;
    }
  }
  return false;
}

boolean readData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return false;
  }
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");
  return true;
}
