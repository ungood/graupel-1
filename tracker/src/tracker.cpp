#include "tracker.h"

#include <Arduino.h>
#include <FileSystem.h>
#include <GPS.h>
#include <Indicators.h>
#include <Sensors.h>
#include <avr/wdt.h>

//#include <Logging.h>
#include <errors.h>

// Initialize all the globals that represent the state of the tracker.
Indicators indicators(PIN_OK, PIN_TX, PIN_ERROR);
Sensors sensors;
FileSystem fs;
GPS gps{Serial3};

void setup() {
  // Enable watchdog timer for 4 seconds.
  wdt_enable(WDTO_4S);

  // Test indicator LEDs
  indicators.setup();
  wdt_reset();

  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) {
  }
  wdt_reset();

  if (!fs.begin()) {
    abort(ERROR_FILESYSTEM_INIT_FAILED, F("Failed to initialize SD cards."));
  }
  wdt_reset();

  // 2. Initialize log file and logging

  // 3. Initialize GPS

  // 4. Initialize radio

  // 5. Initialize RTC

  if (!sensors.begin()) {
    abort(ERROR_SENSOR_INIT_FAILED, F("BME280 sensor failed to initialize."));
  }
}

void loop() { delay(1000); }

void abort(const int errorCode, const __FlashStringHelper* errorMessage) {
  // TODO
  reset();
}