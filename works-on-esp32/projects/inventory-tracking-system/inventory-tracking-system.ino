#include <PN5180.h>
#include <PN5180ISO15693.h>

// Parameters
#define DEFAULT_BLINK_DELAY 50

// Pin Definitions
#define PIN_PN5180_1_NSS  5
#define PIN_PN5180_1_BUSY 17
#define PIN_PN5180_1_RST  27
#define PIN_LED_NFC_READ_RGB_RED 14
#define PIN_LED_NFC_READ_RGB_GREEN 12
#define PIN_LED_NFC_READ_RGB_BLUE 13
#define PIN_LED_ERROR_NFC_READ 15

PN5180ISO15693 nfc15693(PIN_PN5180_1_NSS, PIN_PN5180_1_BUSY, PIN_PN5180_1_RST);

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED_NFC_READ_RGB_RED, OUTPUT);
  pinMode(PIN_LED_NFC_READ_RGB_GREEN, OUTPUT);
  pinMode(PIN_LED_NFC_READ_RGB_BLUE, OUTPUT);
  pinMode(PIN_LED_ERROR_NFC_READ, OUTPUT);

  nfc15693.begin();
}

void loop() {
  uint8_t uid[10];
  
  nfc15693.reset();
  nfc15693.setupRF();
  
  ISO15693ErrorCode rc = nfc15693.getInventory(uid);
  
  if (rc == ISO15693_EC_OK) {
    showRfidReadLedBlink();
    // printTagUid(uid);
    Serial.print(F("ISO-15693 Tag found, UID: "));
  
    for (int i = 0; i < 8; i++) {
      Serial.print(uid[7-i] < 0x10 ? " 0" : " ");
      Serial.print(uid[7-i], HEX); // LSB is first
    }
    showRfidReadLedBlink();
  } else {
    printErrorSerial(rc);
    ledBlink(PIN_LED_ERROR_NFC_READ);
  }

  Serial.println();
  Serial.println(F("----------------------------------"));

  delay(1000); 
}

void printTagUid(uint32_t uid) {

}

void printErrorSerial(ISO15693ErrorCode errorCode) {
  Serial.print(F("*** Error: "));
  Serial.print(errorCode);
  Serial.println("***");
}

void ledBlink(int pinNo) {
  digitalWrite(pinNo, HIGH);
  delay(DEFAULT_BLINK_DELAY);
  digitalWrite(pinNo, LOW);
  delay(DEFAULT_BLINK_DELAY);
}

void showRfidReadLedBlink() {
  ledBlink(PIN_LED_NFC_READ_RGB_RED);
  ledBlink(PIN_LED_NFC_READ_RGB_GREEN);
  ledBlink(PIN_LED_NFC_READ_RGB_BLUE);
}
