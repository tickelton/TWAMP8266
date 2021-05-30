#ifndef __NTPLight_H__
#define __NTPLight_H__

#include <Arduino.h>
#include <WiFiUdp.h>

#undef NTPLight_DEBUG

struct NtpTs {
  uint32_t seconds;
  uint16_t fractions;
};

using NtpTimestamp = struct NtpTs;

constexpr auto kNtpClientDefaultPort = 41423;
constexpr auto kNtpPacketSize = 48;
constexpr auto kNtpUpdateInterval = 60 * 60 * 1000;  // 1h

class NTPLight {
  unsigned long _lastUpdate = 0;
  const char* _ntpServerName = "time.google.com";
  byte _packetBuf[kNtpPacketSize] = {0};
  WiFiUDP _udp;
  NtpTs _lastTimestamp = {0, 0};

 public:
  NTPLight() {}

  void begin();
  bool getTime(NtpTimestamp& ts);
};

#endif  //__NTPLight_H__
