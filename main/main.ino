#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <TWAMP8266.h>
#include <WiFiUdp.h>

constexpr int kBootTimeout = 5000;
constexpr int kLineBufSize = 32;
constexpr int kMaxSsidLength = 33;
constexpr int kMaxPasswordLength = 33;
constexpr int kSerialTimeout = 50;

char line[kLineBufSize];

struct AuthData_t {
  char ssid[kMaxSsidLength];
  char password[kMaxPasswordLength];
} auth_data;
constexpr int kEepromSize = sizeof(auth_data);

TWAMP8266 reflector;

bool read_line() {
  int pos = 0;
  int in_byte;
  while (true) {
    if (Serial.available()) {
      in_byte = Serial.read();
      switch (in_byte) {
        case -1:
          Serial.println("Invalid read.");
          break;
        case '\r':
          [[fallthrough]];
        case '\n':
          line[pos] = '\0';
          Serial.println("");
          while (Serial.available()) {
            Serial.read();
          }
          return true;
          break;
        default:
          if (pos >= kLineBufSize) {
            Serial.println("Input string too long.");
            return false;
          } else {
            line[pos] = in_byte;
            ++pos;
            Serial.printf("%c", in_byte);
          }
          break;
      }
    }
  }
}

bool my_cli() {
  Serial.println("Enter number :");
  Serial.printf(" [1] Change network name [%s].\r\n", auth_data.ssid);
  Serial.println(" [2] Change Password.");
  Serial.println(" [3] Exit and continue startup.");
  Serial.printf("> ");

  if (!read_line()) {
    return true;
  }
  if (strlen(line) != 1) {
    Serial.println("Invalid command.");
    return true;
  }
  int tmpLen = 0;
  switch (line[0]) {
    case '1':
      Serial.printf("Network name > ");
      if (!read_line()) {
        break;
      }
      tmpLen = strlen(line);
      if (tmpLen < 1 || tmpLen >= kMaxSsidLength) {
        Serial.println("Invalid network name.");
      } else {
        memcpy(auth_data.ssid, line, tmpLen);
        auth_data.ssid[tmpLen] = '\0';
        EEPROM.put(0, auth_data);
        EEPROM.commit();
      }
      break;
    case '2':
      Serial.printf("Password > ");
      if (!read_line()) {
        break;
      }
      tmpLen = strlen(line);
      if (tmpLen < 1 || tmpLen >= kMaxPasswordLength) {
        Serial.println("Invalid pasword.");
      } else {
        memcpy(auth_data.password, line, tmpLen);
        auth_data.password[tmpLen] = '\0';
        EEPROM.put(0, auth_data);
        EEPROM.commit();
      }
      break;
    case '3':
      return false;
      break;
    default:
      Serial.println("Invalid command.");
      break;
  }

  memset(line, 0, kLineBufSize);

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(kSerialTimeout);
  pinMode(LED_BUILTIN, OUTPUT);
  EEPROM.begin(kEepromSize);
  EEPROM.get(0, auth_data);
  auth_data.ssid[kMaxSsidLength - 1] = '\0';
  auth_data.password[kMaxPasswordLength - 1] = '\0';

  Serial.println("Press Return to enter WiFi configuration.");

  int start_time = millis();
  while (!Serial.available() && (millis() - start_time) < kBootTimeout) {
    delay(100);
  }

  if (Serial.available()) {
    while (Serial.available()) {
      Serial.read();
    }
    while (my_cli()) {
    }
  }

  WiFi.begin(auth_data.ssid, auth_data.password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  reflector.begin();
}

void loop() { reflector.loop(); }
