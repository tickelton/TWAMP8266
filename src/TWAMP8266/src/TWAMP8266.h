#ifndef __TWAMP8266_H__
#define __TWAMP8266_H__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


void twamp8266_blink();

constexpr uint16_t defaultPort = 862;

class TWAMP8266 {
    uint16_t _listenPort;
  public:
    TWAMP8266() : _listenPort(defaultPort) {}
    TWAMP8266(uint16_t listenPort) : _listenPort(listenPort) {}

    uint16_t getPort() { return _listenPort; }
};


#endif //__TWAMP8266_H__
