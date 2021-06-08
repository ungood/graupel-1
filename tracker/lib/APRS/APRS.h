#ifndef APRS_H
#define APRS_H

#include <Arduino.h>

const byte PROGMEM SINE_TABLE[] = {
    0x80, 0x81, 0x83, 0x84, 0x86, 0x87, 0x89, 0x8A, 0x8C, 0x8D, 0x8F, 0x91, 0x92, 0x94, 0x95, 0x97, 0x98, 0x9A, 0x9B,
    0x9D, 0x9E, 0xA0, 0xA1, 0xA3, 0xA4, 0xA6, 0xA7, 0xA9, 0xAA, 0xAC, 0xAD, 0xAF, 0xB0, 0xB2, 0xB3, 0xB4, 0xB6, 0xB7,
    0xB9, 0xBA, 0xBB, 0xBD, 0xBE, 0xBF, 0xC1, 0xC2, 0xC3, 0xC5, 0xC6, 0xC7, 0xC9, 0xCA, 0xCB, 0xCC, 0xCE, 0xCF, 0xD0,
    0xD1, 0xD2, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDD, 0xDE, 0xDF, 0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
    0xE6, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xEC, 0xED, 0xEE, 0xEF, 0xF0, 0xF0, 0xF1, 0xF2, 0xF2, 0xF3, 0xF4,
    0xF4, 0xF5, 0xF5, 0xF6, 0xF7, 0xF7, 0xF8, 0xF8, 0xF9, 0xF9, 0xF9, 0xFA, 0xFA, 0xFB, 0xFB, 0xFB, 0xFC, 0xFC, 0xFC,
    0xFD, 0xFD, 0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFE, 0xFE, 0xFE, 0xFE,
    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFD, 0xFD, 0xFD, 0xFD, 0xFC, 0xFC, 0xFC, 0xFB, 0xFB, 0xFB, 0xFA, 0xFA, 0xF9,
    0xF9, 0xF9, 0xF8, 0xF8, 0xF7, 0xF7, 0xF6, 0xF5, 0xF5, 0xF4, 0xF4, 0xF3, 0xF2, 0xF2, 0xF1, 0xF0, 0xF0, 0xEF, 0xEE,
    0xED, 0xEC, 0xEC, 0xEB, 0xEA, 0xE9, 0xE8, 0xE7, 0xE6, 0xE6, 0xE5, 0xE4, 0xE3, 0xE2, 0xE1, 0xE0, 0xDF, 0xDE, 0xDD,
    0xDB, 0xDA, 0xD9, 0xD8, 0xD7, 0xD6, 0xD5, 0xD4, 0xD2, 0xD1, 0xD0, 0xCF, 0xCE, 0xCC, 0xCB, 0xCA, 0xC9, 0xC7, 0xC6,
    0xC5, 0xC3, 0xC2, 0xC1, 0xBF, 0xBE, 0xBD, 0xBB, 0xBA, 0xB9, 0xB7, 0xB6, 0xB4, 0xB3, 0xB2, 0xB0, 0xAF, 0xAD, 0xAC,
    0xAA, 0xA9, 0xA7, 0xA6, 0xA4, 0xA3, 0xA1, 0xA0, 0x9E, 0x9D, 0x9B, 0x9A, 0x98, 0x97, 0x95, 0x94, 0x92, 0x91, 0x8F,
    0x8D, 0x8C, 0x8A, 0x89, 0x87, 0x86, 0x84, 0x83, 0x81, 0x80, 0x7E, 0x7C, 0x7B, 0x79, 0x78, 0x76, 0x75, 0x73, 0x72,
    0x70, 0x6E, 0x6D, 0x6B, 0x6A, 0x68, 0x67, 0x65, 0x64, 0x62, 0x61, 0x5F, 0x5E, 0x5C, 0x5B, 0x59, 0x58, 0x56, 0x55,
    0x53, 0x52, 0x50, 0x4F, 0x4D, 0x4C, 0x4B, 0x49, 0x48, 0x46, 0x45, 0x44, 0x42, 0x41, 0x40, 0x3E, 0x3D, 0x3C, 0x3A,
    0x39, 0x38, 0x36, 0x35, 0x34, 0x33, 0x31, 0x30, 0x2F, 0x2E, 0x2D, 0x2B, 0x2A, 0x29, 0x28, 0x27, 0x26, 0x25, 0x24,
    0x22, 0x21, 0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19, 0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x13, 0x12,
    0x11, 0x10, 0x0F, 0x0F, 0x0E, 0x0D, 0x0D, 0x0C, 0x0B, 0x0B, 0x0A, 0x0A, 0x09, 0x08, 0x08, 0x07, 0x07, 0x06, 0x06,
    0x06, 0x05, 0x05, 0x04, 0x04, 0x04, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x08, 0x08, 0x09, 0x0A, 0x0A, 0x0B,
    0x0B, 0x0C, 0x0D, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x19,
    0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2D, 0x2E,
    0x2F, 0x30, 0x31, 0x33, 0x34, 0x35, 0x36, 0x38, 0x39, 0x3A, 0x3C, 0x3D, 0x3E, 0x40, 0x41, 0x42, 0x44, 0x45, 0x46,
    0x48, 0x49, 0x4B, 0x4C, 0x4D, 0x4F, 0x50, 0x52, 0x53, 0x55, 0x56, 0x58, 0x59, 0x5B, 0x5C, 0x5E, 0x5F, 0x61, 0x62,
    0x64, 0x65, 0x67, 0x68, 0x6A, 0x6B, 0x6D, 0x6E, 0x70, 0x72, 0x73, 0x75, 0x76, 0x78, 0x79, 0x7B, 0x7C, 0x7E};

void ax25_send_frame(void *message, size_t message_len);
void ax25_encode_base91(uint32_t value, char *buf, size_t buf_len);
void ax25_encode_latitude(float latitude, char *buf, size_t buf_len);
void ax25_encode_longitude(float longitude, char *buf, size_t buf_len);
void ax25_encode_altitude(float altitude_feet, char *buf, size_t buf_len);
void ax25_encode_callsign(const char *callsign, short ssid, char *buf);

struct AX25Message {
  char destination_address[7]; // Callsign and SSID, AX.25 Encoded - For APRS, this usually identifies the hardware.
  char source_address[7];      // Callsign and SSID, Ax.25 Encoded
  char wide1_address[7];       //
  char wide2_address[7];       //
  char control_field = 0x03;   // UI-Frame
  char protocol_id = 0xf0;     // No Layer 3 protocol

  AX25Message(const char *callsign, short ssid, short ttl1, short ttl2) {
    ax25_encode_callsign("SEA7", 0, destination_address);
    ax25_encode_callsign(callsign, ssid, source_address);
    set_ttls(ttl1, ttl2);
  }

  inline void set_ttls(short ttl1, short ttl2) {
    ax25_encode_callsign("WIDE1", ttl1, wide1_address);
    ax25_encode_callsign("WIDE2", ttl2, wide2_address);

    // Mark the last callsign with a 1 in the LSB.
    wide2_address[6] |= 1;
  }

} __attribute__((packed));

enum APRSSymbol : char { APRS_CAR = '>', APRS_BALLOON = 'O', APRS_MAC = 'M', APRS_BIKE = 'b' };

enum CompressionType : char {
  // Position comes from a current GPS fix.
  COMPRESSION_TYPE_GPS_FIX_CURRENT = 0b00100000,

  // Position is sourced from the GGA sentence - and thus cs bytes are interpreted as altitude.
  COMPRESSION_TYPE_NMEA_SOURCE_GGA = 0b00010000
};

const size_t MAX_COMMENT_LEN = 40;

struct CompressedPositionMessage : AX25Message {
  char message_type = '!';
  char symbol_table_id = '/';
  char compressed_lat[4];
  char compressed_lng[4];
  APRSSymbol symbol;
  char compressed_alt[2];
  // Interpret the previous two bytes as altitude. Adding 33 converts this to a base-91 ASCII character.
  char compression_type = 0b00110000 + 33;
  char comment[MAX_COMMENT_LEN] = "";

  CompressedPositionMessage(const char *callsign, short ssid, short ttl1, short ttl2, APRSSymbol symbol)
      : AX25Message(callsign, ssid, ttl1, ttl2) {
    this->symbol = symbol;
  }

  void set_position(float latitude, float longitude, float altitude_feet);
} __attribute__((packed));

struct APRSConfig {
  byte data_pin;
  byte enable_pin;
  const char *callsign;
  short ssid;
  APRSSymbol symbol;
  const char *comment;
};

class APRS {
private:
  byte data_pin_;
  byte enable_pin_;
  CompressedPositionMessage message_;
  unsigned long nextTransmission_;
  unsigned long transmissionDelay_seconds_;
  unsigned long trasnmissionDelay_jitter_;

public:
  APRS(APRSConfig config)
      : data_pin_{config.data_pin}, enable_pin_{config.enable_pin}, message_{config.callsign, config.ssid, 0, 0,
                                                                             config.symbol} {
    set_position(0, 0, 0);
    set_comment(config.comment);
    // TODO: Set this back to 60s +- 10.
    transmissionDelay_seconds_ = 30UL;
    trasnmissionDelay_jitter_ = 5UL;
    nextTransmission_ = transmissionDelay_seconds_ * 1000;
  }

  bool begin();
  bool loop(unsigned long currentMillis);

  void set_position(float lat, float lng, float alt);
  void set_comment(const char *fmt, ...);
};

#endif