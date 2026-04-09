#include <Wire.h>
#include <Adafruit_PN532.h>

#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Iniciando PN532...");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("No se encontró el PN532");
    while (1);
  }

  Serial.print("Chip PN532 encontrado. Firmware: ");
  Serial.print((versiondata >> 24) & 0xFF, HEX);
  Serial.print(".");
  Serial.println((versiondata >> 16) & 0xFF, HEX);

  nfc.SAMConfig(); // Configura el módulo para leer tarjetas
  Serial.println("Esperando tarjeta...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[7];
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A,
    uid,
    &uidLength
  );

  if (success) {
    Serial.println("Tarjeta detectada:");

    // HEXADECIMAL
    Serial.print("HEX: ");
    for (int i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // DECIMAL
    Serial.print("DEC: ");
    for (int i = 0; i < uidLength; i++) {
      Serial.print(uid[i], DEC);
      Serial.print(" ");
    }
    Serial.println();

    Serial.print("BIN: ");
    for (int i = 0; i < uidLength; i++) {
      for (int bit = 7; bit >= 0; bit--) {
        Serial.print((uid[i] >> bit) & 1);
      }
      Serial.print(" ");
    }
    Serial.println();

    Serial.println("------------------------");
    delay(1500);
  }
}