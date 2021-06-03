#include "NTPLight.h"

void NTPLight::begin() { _udp.begin(kNtpClientDefaultPort); }

bool NTPLight::getTime(NtpTimestamp& ts) {
  if (!_lastUpdate || (millis() - _lastUpdate) > kNtpUpdateInterval) {
    // discard existing packets
    while (_udp.parsePacket() != 0) {
      _udp.flush();
    }

    memset(_packetBuf, 0, kNtpPacketSize);
    _packetBuf[0] = 0b11100011;  // LI, Version, Mode
    _packetBuf[1] = 0;           // Stratum, or type of clock
    _packetBuf[2] = 6;           // Polling Interval
    _packetBuf[3] = 0xEC;        // Peer Clock Precision
    _packetBuf[12] = 49;
    _packetBuf[13] = 0x4E;
    _packetBuf[14] = 49;
    _packetBuf[15] = 52;

    _udp.beginPacket(_ntpServerName, 123);
    _udp.write(_packetBuf, kNtpPacketSize);
    _udp.endPacket();

    // Wait till data is there or timeout...
    int timeout = 0;
    int cb = 0;
    do {
      _lastUpdate = millis();
      delay(1);
      cb = _udp.parsePacket();
      if (timeout > 1000) return false;  // timeout after 1000 ms
      timeout++;
    } while (cb == 0);

#if defined(NTPLight_DEBUG)
    Serial.println("Received NTP data.");
#endif  // NTPLight_DEBUG

    _udp.read(_packetBuf, kNtpPacketSize);

    unsigned long highWord = word(_packetBuf[40], _packetBuf[41]);
    unsigned long lowWord = word(_packetBuf[42], _packetBuf[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

#if defined(NTPLight_DEBUG)
    Serial.printf("secsSince1900 = %lu\r\n", secsSince1900);
#endif  // NTPLight_DEBUG

    highWord = word(_packetBuf[44], _packetBuf[45]);
    uint16_t transmit_dec = highWord;

#if defined(NTPLight_DEBUG)
    Serial.printf("fractions = %u\r\n", transmit_dec);
#endif  // NTPLight_DEBUG

    _lastTimestamp = {.seconds = secsSince1900, .fractions = transmit_dec};
  }

  auto timeDiff = millis() - _lastUpdate;
  auto secDiff = timeDiff / 1000;
  auto msDiff = timeDiff - ((timeDiff / 1000) * 1000);

  unsigned long tsTmp = _lastTimestamp.fractions + ((msDiff * 65536) / 1000);
  ts.fractions = tsTmp % 65536;
  ts.seconds = _lastTimestamp.seconds + secDiff + (tsTmp >> 16);
#if defined(NTPLight_DEBUG)
  Serial.printf("timeDiff = %lu\r\n", timeDiff);
  Serial.printf("secDiff = %lu\r\n", secDiff);
  Serial.printf("msDiff = %lu\r\n", msDiff);
  Serial.printf("tsTmp = %lu\r\n", tsTmp);
  Serial.printf("ts.fractions = %u\r\n", ts.fractions);
  Serial.printf("ts.seconds = %u\r\n", ts.seconds);
#endif  // NTPLight_DEBUG

  return true;
};
