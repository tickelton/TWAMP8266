#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <twamp8266.h>

const char *ssid     = "<SSID>";
const char *password = "<PASSWORD>";

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP);


void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(BUILTIN_LED, OUTPUT);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());

  delay(1000);

  twamp8266_blink();
}
