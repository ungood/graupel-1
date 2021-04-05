#ifndef GPS_H
#define GPS_H

// #define NMEAGPS_PARSE_GGA  // Global Positioning System Fix Data
// #define NMEAGPS_PARSE_GST  // Position error statistics
// #define NMEAGPS_PARSE_RMC  // Recommended minimum specific GPS/Transit data

// // Enables most of the fields that NeoGPS can parse.
// #define GPS_FIX_DATE
// #define GPS_FIX_TIME
// #define GPS_FIX_LOCATION
// #define GPS_FIX_LOCATION_DMS
// #define GPS_FIX_ALTITUDE
// #define GPS_FIX_SPEED
// #define GPS_FIX_HEADING
// #define GPS_FIX_SATELLITES
// #define GPS_FIX_HDOP
// #define GPS_FIX_LAT_ERR
// #define GPS_FIX_LON_ERR
// #define GPS_FIX_ALT_ERR
// #define GPS_FIX_GEOID_HEIGHT

// #define NMEAGPS_DERIVED_TYPES
// #define NMEAGPS_PARSE_PROPRIETARY
// #define NMEAGPS_PARSE_MFR_ID
// #include <ublox/ubxGPS.h>

#include <TinyGPS++.h>

#include "ubxmsg.h"

class GPS {
private:
  HardwareSerial &port_;

  void write(const ublox::msg_t &message);
  bool waitForAcknowledge(ublox::msg_class_t msgClass, ublox::msg_id_t msgId, const unsigned int timeout);

public:
  // TODO: Encapsulate
  TinyGPSPlus gps_;

  GPS(HardwareSerial &port) : port_{port} {}
  bool begin();
  bool send(const ublox::msg_t &message, const unsigned int timeout = 2000);
  void loop(unsigned long currentMillis);
};

#endif