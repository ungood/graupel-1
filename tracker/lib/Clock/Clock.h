#ifndef CLOCK_H
#define CLOCK_H

#include <RTClib.h>
#include <TinyGPS++.h>
#include <WString.h>

const unsigned long TIMESTAMP_PERIOD = 1000;

class Clock {
private:
  RTC_PCF8523 rtc_;
  bool synced_to_gps_ = false;
  unsigned long next_timestamp_ = TIMESTAMP_PERIOD;

public:
  bool begin();
  inline DateTime now() {
    DateTime now = rtc_.now();
    delay(2);
    return now;
  }
  void synchronize(TinyGPSDate &date, TinyGPSTime &time);
  void loop(unsigned long currentMillis);
  void trace(DateTime& now);
};

#endif