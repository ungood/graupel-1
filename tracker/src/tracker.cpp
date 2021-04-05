#include "tracker.h"

#include <Arduino.h>
#include <ArduinoLog.h>
#include <avr/wdt.h>

#include <FileSystem.h>
#include <GPS.h>
#include <Indicators.h>
#include <Sensors.h>

#include <config.h>
#include <errors.h>

#include "tracker.h"

void setupIndicators() {
  indicators.begin();
  indicators.on();

  delay(500);

  indicators.off();
  wdt_reset();
}

void setupLogging() {
  Serial.begin(9600);
  while (!Serial) {
    yield();
  }

  Log.begin(LOG_LEVEL, &DEBUG_STREAM, true);
  Log.verbose(F("Logging initialized.\n"));
  wdt_reset();
}

void setupFilesystem() {
  if (!fs.begin()) {
    abort(ERROR_FILESYSTEM_INIT_FAILED, F("Failed to initialize SD cards."));
  }
  wdt_reset();
}

void setup() {
  // Enable watchdog timer for 4 seconds.
  wdt_enable(WDTO_4S);

  setupIndicators();

  // Initializing, Green light blinking!
  indicators.ok.blink(1000, 0xFF00);
  setupLogging();
  //abort(5, F("TESTING ABORT"));

  //setupFilesystem();

  // 2. Initialize log file and logging

  // 3. Initialize GPS

  // 4. Initialize radio

  // 5. Initialize RTC

  // if (!sensors.begin()) {
  //   abort(ERROR_SENSOR_INIT_FAILED, F("BME280 sensor failed to initialize."));
  // }

  // Initialized, green light on
  indicators.ok.on();

  indicators.tx.blink(2000, 0xAAAA);
  indicators.error.on();
}

unsigned long currentMillis = 0;

void loop() {
  currentMillis = millis();

  indicators.loop(currentMillis);
  wdt_reset();
}

void abort(unsigned int errorCode, const __FlashStringHelper *errorMessage) {
  Log.fatal(errorMessage);
  indicators.off();
  indicators.error.blink(5000, errorCode);

  unsigned long currentMillis = millis();
  unsigned long resetAt = currentMillis + (10 * 1000);

  while (currentMillis < resetAt) {
    indicators.loop(currentMillis);
    currentMillis = millis();
  }

  reset();
}