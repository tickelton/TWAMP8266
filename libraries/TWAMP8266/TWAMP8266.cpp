#include "TWAMP8266.h"

void TWAMP8266::begin() {
  _udp.begin(_listenPort);
  _ntp.begin();
#if defined(TWAMP8266_DEBUG)
  auto ret = _ntp.getTime(_receiveTs);
  if (ret) {
    Serial.printf("NTP update success: sec=%lu frac=%u\r\n", _receiveTs.seconds,
                  _receiveTs.fractions);
  } else {
    Serial.println("NTP updated failed!");
  }
#else
  _ntp.getTime(_receiveTs);
#endif  // TWAMP8266_DEBUG
}

void TWAMP8266::loop() {
  // Check if there's a packet available.
  int packetSize = _udp.parsePacket();
  if (packetSize) {
    // TODO: get receive timestamp
    auto ntpRet = _ntp.getTime(_receiveTs);
    if (!ntpRet) {
      Serial.println("Error getting receive timestamp.");
      _receiveTs = {0, 0};
    }

#if defined(TWAMP8266_DEBUG)
    Serial.printf("Received %d bytes from %s\r\n", packetSize,
                  _udp.remoteIP().toString().c_str());
#endif  // TWAMP8266_DEBUG

    // Get package data from the receive buffer.
    auto len = _udp.read(_recvBuf, maxPacketSize);
    if (len < 0) {
      Serial.println("Error: No data available.");
    } else if (0 == len) {
#if defined(TWAMP8266_DEBUG)
      Serial.println("Received empty packet.");
#endif  // TWAMP8266_DEBUG
    } else if (len < minRequestLength) {
      // The packet received was shorten than the shortest
      // valid TWAMP package possible so we're discarding it.
      Serial.println("Error: Short receive.");
    } else {
      // TWAMP request received, now we build a response packet.

      // Set response sequence number.
      uint32_t longVal = htonl(_sequenceNr);
      memcpy(_sendBuf, &longVal, 4);

      // Set error estimate.
      memcpy(_sendBuf + 12, &_errorEstimate, 2);

      // set receive timestamp
      longVal = htonl(_receiveTs.seconds);
      memcpy(_sendBuf + 16, &longVal, 4);
      uint16_t shortVal = htons(_receiveTs.fractions);
      memcpy(_sendBuf + 20, &shortVal, 2);

      // Copy request packet to the appropriate location in the
      // response packet.
      memcpy(_sendBuf + 24, _recvBuf, minRequestLength);
      memcpy(_sendBuf + 40, &senderTTL, 1);

      // TODO: get send timestamp
      ntpRet = _ntp.getTime(_sendTs);
      if (!ntpRet) {
        Serial.println("Error getting send timestamp.");
        _sendTs = {0, 0};
      }

      // set send timestamp
      longVal = htonl(_sendTs.seconds);
      memcpy(_sendBuf + 4, &longVal, 4);
      shortVal = htons(_sendTs.fractions);
      memcpy(_sendBuf + 8, &shortVal, 2);

      // send packet
      _udp.beginPacket(_udp.remoteIP(), _udp.remotePort());
      _udp.write(_sendBuf, minResponseLength);
      _udp.endPacket();

      // Clear buffers so Must-Be-Zero fields are initialized
      // properly for the next packet and we don't have to
      // handle them separately.
      memset(_recvBuf, 0, maxPacketSize);
      memset(_sendBuf, 0, maxPacketSize);
      _sequenceNr++;
    }
  } else {
    // If there's no packet to process, update NTP time.
    // An actual request the the NTP server is only send
    // if the update interval is expired.
    auto ntpRet = _ntp.getTime(_receiveTs);
    if (!ntpRet) {
      Serial.println("Error updating NTP time.");
    }
  }
}
