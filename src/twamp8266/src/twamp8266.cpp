#include <Arduino.h>
#include "twamp8266.h"

void twamp8266_blink() {
  digitalWrite(BUILTIN_LED, HIGH);
  delay(100);
  digitalWrite(BUILTIN_LED, LOW);
  delay(100);
}
