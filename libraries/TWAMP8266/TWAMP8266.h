#ifndef __TWAMP8266_H__
#define __TWAMP8266_H__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPLight.h>
#include <WiFiUdp.h>

//
// TWAMP test packet format according to RFC5357:
//
// 0                   1                   2                   3
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                        Sequence Number                        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                          Timestamp                            |
// |                                                               |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |         Error Estimate        |           MBZ                 |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                          Receive Timestamp                    |
// |                                                               |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                        Sender Sequence Number                 |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |                      Sender Timestamp                         |
// |                                                               |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |      Sender Error Estimate    |           MBZ                 |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |  Sender TTL   |                                               |
// +-+-+-+-+-+-+-+-+                                               +
// |                                                               |
// .                                                               .
// .                         Packet Padding                        .
// .                                                               .
// |                                                               |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#undef TWAMP8266_DEBUG

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
  NtpTs _receiveTs = {0, 0};
  NtpTs _sendTs = {0, 0};

 public:
  TWAMP8266() : _listenPort(twlDefaultPort) {}
  TWAMP8266(int listenPort) : _listenPort(listenPort) {}

  void begin();
  void loop();
};

#endif  //__TWAMP8266_H__
