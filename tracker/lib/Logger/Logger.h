#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <ArduinoLog.h>
#include <Print.h>

#include <config.h>

class NopPrint : public Print {
  inline size_t write(uint8_t chr) { return 1; }
};

/**
 * Implementation of Print that can swap out different base implementations of
 * Print. Used to allow us to swap between Serial and SD-card logging.
 */
class HotSwappablePrint : public Print {
 private:
  Print& delegate_;

 public:
  HotSwappablePrint(Print& initialDelegate) : delegate_{initialDelegate} {}
  inline size_t write(uint8_t chr) { return delegate_.write(chr); }
  inline void setDelegate(Print& delegate) { delegate_ = delegate; }
};

class SwappableLogger : public Logging {
 private:
  HotSwappablePrint output_;

 public:
  SwappableLogger(Print& output) : output_{output} {}
  inline void begin() { Logging::begin(LOG_LEVEL, &output_); }
  inline void setOutput(Print& output) { output_.setDelegate(output); }
};

extern SwappableLogger logger;

#endif