#include <twamp8266.h>

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  twamp8266_blink();
}
