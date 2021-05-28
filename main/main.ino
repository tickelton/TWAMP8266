#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <TWAMP8266.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#undef FORCE_EEPROM_RESET

constexpr auto kMaxSsidLength = 33;
constexpr auto kMaxPasswordLength = 33;

struct authData_t {
    char ssid[kMaxSsidLength];
    char password[kMaxPasswordLength];
    bool valid;
} authData;

constexpr auto kEepromSize = sizeof(authData);

// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP);
TWAMP8266 reflector;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

    memset(authData.ssid, 0,  kMaxSsidLength);
    memset(authData.password, 0,  kMaxPasswordLength);
    authData.valid = false;
    EEPROM.begin(kEepromSize);

#if defined(FORCE_EEPROM_RESET)
    delay(10000);
    Serial.println("WARNING: Resetting EEPROM in 10s!");
    delay(10000);
    EEPROM.put(0, authData);
    EEPROM.commit();
#endif // FORCE_EEPROM_RESET


    delay(5000);
    Serial.println("Reading EEPROM...");
    EEPROM.get(0, authData);
    authData.ssid[kMaxSsidLength-1] = '\0';
    authData.password[kMaxPasswordLength-1] = '\0';
    Serial.printf("Got data: ssid=%s, valid=%d\n", authData.ssid, authData.valid);

    if (! authData.valid) {
        while (Serial.available() > 0) {
        Serial.print("read:");
        Serial.print(Serial.read());
        }
        Serial.print("SSID: ");
        Serial.readBytesUntil('\n', authData.ssid, kMaxSsidLength - 1);
        Serial.print("\nPassword: ");
        Serial.readBytesUntil('\n', authData.password, kMaxPasswordLength - 1);
        authData.valid = true;
    }

  WiFi.begin(authData.ssid, authData.password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  //  timeClient.begin();

  reflector.begin();
}

void loop() {
  //  timeClient.update();
  //  Serial.println(timeClient.getFormattedTime());
  //  delay(1000);

  reflector.loop();
}
