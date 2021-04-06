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

// PINS IN USE
// 2 - Launch button!
// 3 - SD Shield LED1 (Green)
// 4 - SD Shield LED2 (Red)
// 7 - OK LED         (Green)
// 8 - ERROR LED      (Red)
// 9 - Radio Data
// 10 - SD CHIP SELECT
// 11 - SD MOSI
// 12 - SD MISO
// 13 - SD SCK
// 22 - TX LED        (Yellow)

// A3 - Radio Enable
// A4 - RTC SCL
// A5 - RTC SDA

// Indicators
const unsigned int PIN_OK    =  8; // Digital pin for the OK LED
const unsigned int PIN_TX    = 22; // Digital pin for the TX LED
const unsigned int PIN_ERROR =  7; // Digital pin for the ERROR LED
Indicators indicators{PIN_OK, PIN_TX, PIN_ERROR};

const SdCsPin_t PIN_CHIP_SELECT = 10;
const unsigned int PIN_MOSI = 11;
const unsigned int PIN_MISO = 12;
const unsigned int PIN_SCK = 13;

SoftSpiDriver<PIN_MOSI, PIN_MOSI, PIN_SCK> softSpi;
// Speed argument is ignored for software SPI.
SdSpiConfig sdConfig(PIN_CHIP_SELECT, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi);
FileSystem filesystem{sdConfig};

// Real Time Clock

// Sensors
Sensors sensors{Wire, 0x76};

GPS gps{Serial3};

// APRS
APRSConfig aprs_config{
  .data_pin = 9,
  .enable_pin = A3,
  .callsign = "KJ7VPV", // Change this, do not use mine please!
  .ssid = 9, // 9 is good for testing, 11 for balloons.
  .symbol = APRS_MAC,
  .comment = "Graupel-1 HAB: https://git.io/JYQqO", // 40 characters or less
};
APRS aprs{aprs_config};

#endif