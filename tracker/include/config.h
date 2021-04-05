#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <APRS.h>
#include <FileSystem.h>
#include <GPS.h>
#include <Indicators.h>
#include <Sensors.h>

// Serial port is not available when unit testing.
#ifdef UNIT_TEST
#define DISABLE_LOGGING
#endif

Stream& DEBUG_STREAM = Serial;

// Indicators
const unsigned int PIN_OK    =  8; // Digital pin for the OK LED
const unsigned int PIN_TX    = 22; // Digital pin for the TX LED
const unsigned int PIN_ERROR =  7; // Digital pin for the ERROR LED
Indicators indicators{PIN_OK, PIN_TX, PIN_ERROR};

// SD Card Shield
const unsigned int PIN_CHIP_SELECT = 10;

// Sensors
Sensors sensors{Wire, 0x76};

FileSystem fs;
GPS gps{Serial3};

// APRS
APRSConfig aprs_config{
  .data_pin = 9,
  .enable_pin = A3,
  .callsign = "KJ7VPV",
  .ssid = 9, // 9 is good for testing, 11 for balloons.
  .symbol = APRS_MAC,
  .comment = "Hello, World!",
};
APRS aprs{aprs_config};

#endif