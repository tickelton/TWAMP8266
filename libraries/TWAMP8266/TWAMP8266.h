#ifndef __TWAMP8266_H__
#define __TWAMP8266_H__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPLight.h>
#include <WiFiUdp.h>

//#undef TWAMP8266_DEBUG
#define TWAMP8266_DEBUG

constexpr auto maxPacketSize = 4096;
constexpr auto minRequestLength = 14;
constexpr auto minResponseLength = 41;
constexpr auto twlDefaultPort = 862;
constexpr uint8_t senderTTL = 255;

class TWAMP8266 {
  const uint16_t _errorEstimate = htons(1);

  int _listenPort;
  int _sequenceNr = 0;
  char _recvBuf[maxPacketSize] = {0};
  char _sendBuf[maxPacketSize] = {0};
  WiFiUDP _udp;
  NTPLight _ntp;
  NtpTs _ts = {0, 0};

 public:
  TWAMP8266() : _listenPort(twlDefaultPort) {}
  TWAMP8266(int listenPort) : _listenPort(listenPort) {}

  void begin();
  void loop();
};

#endif  //__TWAMP8266_H__
