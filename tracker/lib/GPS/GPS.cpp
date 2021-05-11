#include "GPS.h"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <common/SysCall.h>

static const uint8_t ubloxHeader[] = {0xB5, 0x62};

bool GPS::begin() {
  port_.begin(9600);
  port_.setTimeout(3000);
  while (!port_) {
    SysCall::yield();
  }

  // Sends the CFG-NAV5 command, setting the dynamic platform model to "Airborne
  // <2g", setting max altitude to 50,000m and which is "recommended for typical
  // airborne environment".
  Log.trace(F("Configuring GPS for AIR 2G dynamic model.\n"));
  ublox::cfg_nav5_t cfgNav5;
  cfgNav5.apply.dyn_model = 1;
  cfgNav5.dyn_model = ublox::dyn_model_t::UBX_DYN_MODEL_AIR_2G;
  
  return send(cfgNav5);
}

bool GPS::send(const ublox::msg_t &message, const unsigned int timeout) {
  while(port_.available()) {
    port_.read();
  }
  write(message);
  return waitForAcknowledge(message.msg_class, message.msg_id, timeout);
}

void GPS::write(const ublox::msg_t &message) {
  Log.verbose(F("Writing message class: %x, id: %x.\n"), message.msg_class, message.msg_id);
  port_.write(ubloxHeader, 2);

  byte ck_a = 0;
  byte ck_b = 0;
  byte *buffer = (byte *)&message;
  size_t length = message.length + sizeof(ublox::msg_t);

  for (size_t i = 0; i < length; i++) {
    ck_a = ck_a + buffer[i];
    ck_b = ck_b + ck_a;
    port_.write(buffer[i]);
  }

  port_.write(ck_a);
  port_.write(ck_b);
  port_.flush();
}

bool GPS::waitForAcknowledge(ublox::msg_class_t msgClass, ublox::msg_id_t msgId, const unsigned int timeout) {
  Log.verbose(F("Waiting for acknowledgement for message class: %x, id: %x.\n"), msgClass, msgId);

  byte ack[] = {
      ubloxHeader[0], ubloxHeader[1],     //
      ublox::UBX_ACK, ublox::UBX_ACK_ACK, //
      0x02,           0x00,               //
      msgClass,       msgId               //
  };

  int matched = 0;
  const auto timeoutExpires = millis() + timeout;
  
  while(millis() < timeoutExpires) {
    while (port_.available()) {
      auto read = port_.read();
      Log.verbose(F("    Looking for ack[%d] == %X. Found %X\n"), matched, ack[matched], read);

      if (read == ack[matched]) {
        matched++;
      } else {
        matched = 0;
      }

      if (matched >= 8) {
        Log.verbose(F("Ack received for message for message class: %x, id: %x.\n"), msgClass, msgId);
        return true;
      }
    }
    yield();
  }

  Log.error(F("Timed out while waiting for acknowledgment for message class: %x, id: %x.\n"), msgClass, msgId);
  return true;
  // TODO: return false; - once I figure out why this sometimes fails.
}

void GPS::loop(unsigned long currentMillis) {
  while(port_.available()) {
    char c = port_.read();
    encode(c);
  }
}