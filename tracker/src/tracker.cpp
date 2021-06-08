#include <Arduino.h>
#include <ArduinoLog.h>
#include <WString.h>
#include <avr/wdt.h>

#include <FileSystem.h>
#include <GPS.h>
#include <Indicators.h>
#include <Sensors.h>

#include <config.h>
#include <record.h>

/**
 * Forces a hard reset of the device by jumping to address 0.
 */
void (*reset)(void) = 0;

const unsigned int blinkPatterns[] = {0x0000, 0x0001, 0x0005, 0x0015, 0x0055, 0x0155, 0x0555, 0x1555, 0x5555};
const size_t blinkPatternsLength = 9;
unsigned long currentMillis = 0;
TelemetryRecord record;

/**
 * Something bad has happened.
 */
void abort(unsigned int errorCode, const __FlashStringHelper *errorMessage) {
  Log.fatal(errorMessage);
  indicators.off();
  indicators.error.blink(5000, blinkPatterns[errorCode]);

  unsigned long currentMillis = millis();
  unsigned long resetAt = currentMillis + (30 * 1000);

  while (currentMillis < resetAt) {
    indicators.loop(currentMillis);
    currentMillis = millis();
    wdt_reset();
  }

  reset();
}

/**
 * Call back for file timestamps.
 */
void timestamp(uint16_t *date, uint16_t *time) {
  DateTime now = clock.now();

  // Return date using FS_DATE macro to format fields.
  *date = FS_DATE(now.year(), now.month(), now.day());

  // Return time using FS_TIME macro to format fields.
  *time = FS_TIME(now.hour(), now.minute(), now.second());
}


void setupLogging() {
  Serial.begin(9600);
  while (!Serial) {
    yield();
  }

  Log.begin(LOG_LEVEL, &DEBUG_STREAM, true);

  Log.trace(F("Logging initialized.\n"));
  wdt_reset();
}

void setupIndicators() {
  indicators.begin();
  indicators.on();

  Log.verbose(F("Testing indicator LEDs.\n"));
  delay(500);

  indicators.off();

  Log.trace(F("Indicators initialized.\n"));
  wdt_reset();
}

void setupClock() {
  if (!clock.begin()) {
    abort(1, F("Failed to initialize RTC!\n"));
  }

  Log.trace(F("RTC initialized.\n"));
  wdt_reset();
}

void setupFilesystem() {
  // SdFile::dateTimeCallback(timestamp);
  if (!filesystem.begin(record)) {
    abort(2, F("Failed to initialize SD card!\n"));
  }

  Log.trace(F("Filesystem initialized.\n"));
  wdt_reset();
}

void setupGPS() {
  if (!gps.begin()) {
    abort(3, F("Failed to initialize GPS!\n"));
  }

  Log.trace(F("GPS initialized.\n"));
  wdt_reset();
}

void setupSensors() {
  if (!sensors.begin()) {
    abort(4, F("Failed to initialize sensors!\n"));
  }

  Log.trace(F("Sensors initialized.\n"));
  wdt_reset();
}

void setupRadio() {
  if (!aprs.begin()) {
    abort(5, F("Failed to initialize APRS radio!\n"));
  }

  Log.trace(F("Radio initialized.\n"));
  wdt_reset();
}

void setup() {
  // Enable watchdog timer for ~8 seconds.
  wdt_enable(WDTO_8S);

  setupLogging();
  setupIndicators();

  // Unfortunately, something about reading the RTC seems to interfere with reading the temp/humidity
  // sensor and cause the Arduino to hang, so it's removed for now.
  // setupClock();

  setupFilesystem();
  setupGPS();
  setupSensors();
  setupRadio();

  // Analog A0 reads the arduino's battery level.
  analogReference(DEFAULT);
  pinMode(A0, INPUT);

  // Initialized, green light on
  indicators.ok.on();
  Log.notice(F("Initialization successful!.\n"));
}

unsigned long nextTelemetry = 0;

double readVoltage(uint8_t pin, double multiplier) {
  int sensorValue = analogRead(pin);
  double voltage = sensorValue * (5.0 / 1023.0);
  return (voltage * multiplier);
}

void loop() {
  currentMillis = millis();
  record.millis.set(currentMillis);

  // Blink the lights!
  indicators.loop(currentMillis);

  // Read the GPS
  gps.loop(currentMillis);

  // Synchronize the RTC clock if needed.
  // if(gps.date.isValid() && gps.time.isValid()) {
  //   clock.synchronize(gps.date, gps.time);
  // }

  // Blink the number of tracked satellites.
  if(gps.satellites.isValid()) {
    uint32_t satellites = gps.satellites.value();

    if(satellites >= blinkPatternsLength) {
      indicators.tx.on();
    } else {
      indicators.tx.blink(2000, blinkPatterns[satellites]);
    }
  }

  if(gps.date.isValid()) {
    record.year.set(gps.date.year());
    record.month.set(gps.date.month());
    record.day.set(gps.date.day());
  }

  if(gps.time.isValid()) {
    record.hour.set(gps.time.hour());
    record.minute.set(gps.time.minute());
    record.second.set(gps.time.second());
  }

  if(gps.satellites.isValid()) {
    record.satellites.set(gps.satellites.value());
  }

  if(gps.hdop.isValid()) {
    record.hdop.set(gps.hdop.hdop());
  }

  if(gps.altitude.isValid()) {
    record.gps_altitude_m.set(gps.altitude.meters());
  }

  if(gps.course.isValid()) {
    record.course.set(gps.course.deg());
  }

  if(gps.speed.isValid()) {
    record.speed_mps.set(gps.speed.mps());
  }

  if(gps.location.isValid()) {
    record.latitude.set(gps.location.lat());
    record.longitude.set(gps.location.lng());
  }

  // Update the APRS tracker's location.
  if(gps.location.isValid()) {
    aprs.set_position(gps.location.lat(), gps.location.lng(), gps.altitude.feet());
  }

  static SensorReading reading;
  if(sensors.read(reading)) {
    record.temperature_C.set(reading.temperature_C);
    record.pressure_Pa.set(reading.pressure_Pa);
    record.pressure_altitude_m.set(reading.altitude_m);
    record.humidity_RH.set(reading.humidity_RH);
  }

  if(aprs.loop(currentMillis)) {
    Log.verbose(F("APRS Packet transmitted!\n"));
  }

  if(currentMillis > nextTelemetry) {
    record.voltage.set(readVoltage(PIN_A0, 6.0));
    nextTelemetry = currentMillis + 1000; // Log once a second.
    Log.verbose(F("Logging telemetry.\n"));
    filesystem.write(record);

    record.write(Serial);
  }

  wdt_reset();
}