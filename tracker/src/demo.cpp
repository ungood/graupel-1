#include <Arduino.h>
#include <Indicators.h>

#include <config.h>
#include <ArduinoLog.h>

const unsigned int blinkPatterns[] = {
  0x0000,
  0x0001,
  0x0005,
  0x0015,
  0x0055,
  0x0155,
  0x0555,
  0x1555,
  0x5555
};
const size_t blinkPatternsLength = 9;

void setup() {
  Serial.begin(115200);
  while(!Serial) {}

  Log.begin(LOG_LEVEL, &DEBUG_STREAM);
  Log.trace(F("Logging initialized.\n"));

  clock.begin();

  indicators.begin();
  indicators.ok.blink(1000, 0x00FF);
  Log.trace(F("Indicators initialized.\n"));

  gps.begin();
  Log.trace(F("GPS initialized.\n"));
  
  aprs.begin();
  Log.trace(F("Radio initialized.\n"));

  Log.notice(F("Initialization successful!.\n"));
  indicators.ok.on();
  indicators.error.on();

  if(!sensors.begin()) {
    Log.warning(F("Failed to initialize sensors!\n"));
  }

  if(!filesystem.begin()) {
    Log.warning(F("Failed to initialize filesystem!\n"));
  }

  pinMode(PIN2, INPUT_PULLUP);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT);
  digitalWrite(PIN3, HIGH);
  digitalWrite(PIN4, HIGH);
}

unsigned long currentMillis = millis();
unsigned long nextTrans = 0;
unsigned long nextTimestamp = 0;
unsigned int satellites = 0;

void loop() {
  currentMillis = millis();
  indicators.loop(currentMillis);

  gps.loop(currentMillis);

  if(gps.gps_.satellites.isValid()) {
    satellites = gps.gps_.satellites.value();

    if(satellites >= blinkPatternsLength) {
      indicators.tx.on();
    } else {
      indicators.tx.blink(2000, blinkPatterns[satellites]);
    }
  }

  // if(gps.gps_.location.isValid()) {
  //   aprs.set_position(gps.gps_.location.lat(), gps.gps_.location.lng(), gps.gps_.altitude.feet());
  // }

// this appears to be a problem!
  static SensorReading reading;
  sensors.read(reading);
  //Serial.println(reading.altitude_m);

  // if(aprs.loop(currentMillis)) {
  //   Log.verbose(F("Transmitted! Satellites found: %d\n"), satellites);
  //   Log.verbose(F("Altitude - GPS: %d, Pressure: %d\n"), (int)gps.gps_.altitude.feet(), (int)reading.altitude_m);
  // }

  if(currentMillis > nextTimestamp) {
    clock.logTime();
    nextTimestamp = currentMillis + 500;
  }
}
