#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <TWAMP8266.h>
#include <WiFiUdp.h>

const char *ssid = "<SSID>";
const char *password = "<PASSWORD>";

// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP);
TWAMP8266 reflector;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(ssid, password);
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
