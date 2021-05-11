#include <ArduinoLog.h>
#include <TinyGPS++.h>

#include "Clock.h"

bool Clock::begin() {
  if (!rtc_.begin()) {
    Log.error(F("RTC failed to initialize.\n"));
    return false;
  }

  if (!rtc_.initialized() || rtc_.lostPower()) {
    // This will set the RTC to the date and time this program was uploaded, which isn't terribly accurate
    // but a decent place to bootstratp.  Later, we'll synchronize the RTC to GPS time, which is incredibly
    // accurate.
    DateTime uploaded(F(__DATE__), F(__TIME__));
    Log.trace(F("The RTC is not initialized, resetting RTC's date and time:\n"));
    trace(uploaded);
    rtc_.adjust(uploaded);
    Log.trace(F("Done resetting RTC's date and time.\n"));
  }

  // When the RTC was stopped and stays connected to the battery, it has
  // to be restarted by clearing the STOP bit. Let's do this to ensure
  // the RTC is running.
  rtc_.start();

  delay(2);
  return true;
}

void Clock::synchronize(TinyGPSDate &date, TinyGPSTime &time) {
  if(!synced_to_gps_ && date.isValid() && time.isValid()) {
    DateTime newTime(date.year(), date.month(), date.day(), time.hour(), time.minute(), time.second());
    Log.trace(F("Synchronizing RTC clock to GPS time:\n"));
    trace(newTime);
    synced_to_gps_ = true;
  }
}

void Clock::loop(unsigned long currentMillis) {
  if(currentMillis > next_timestamp_) {
    next_timestamp_ = currentMillis + TIMESTAMP_PERIOD;
    DateTime now = rtc_.now();
    trace(now);
  }
}

void Clock::trace(DateTime& now) {
  char buffer[] = "YYYY-MM-DD hh:mm:ss\n";
  now.toString(buffer);
  Log.trace(buffer);
}