#ifndef GPS_H
#define GPS_H

#define NMEAGPS_PARSE_GGA  // Global Positioning System Fix Data
#define NMEAGPS_PARSE_GST  // Position error statistics
#define NMEAGPS_PARSE_RMC  // Recommended minimum specific GPS/Transit data

// Enables most of the fields that NeoGPS can parse.
#define GPS_FIX_DATE
#define GPS_FIX_TIME
#define GPS_FIX_LOCATION
#define GPS_FIX_LOCATION_DMS
#define GPS_FIX_ALTITUDE
#define GPS_FIX_SPEED
#define GPS_FIX_HEADING
#define GPS_FIX_SATELLITES
#define GPS_FIX_HDOP
#define GPS_FIX_LAT_ERR
#define GPS_FIX_LON_ERR
#define GPS_FIX_ALT_ERR
#define GPS_FIX_GEOID_HEIGHT

#include <NMEAGPS.h>

// Merges NMEA sentences into one accumulated "fix", even if that makes data
// incoherent.
#define NMEAGPS_IMPLICIT_MERGING

class GPS {
 private:
  NMEAGPS gps_;
  HardwareSerial port_;

 public:
  GPS(HardwareSerial port) : port_{port} {}
};

#endif