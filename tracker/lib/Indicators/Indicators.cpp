#include <Arduino.h>
#include <ArduinoLog.h>

#include "Indicators.h"

// Fast integer ceiling
// https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c
int ceil_div(int x, int y) {
    return x / y + (x % y > 0);
}

void LED::begin(bool initialValue) {
  pinMode(pin_, OUTPUT);
  digitalWrite(pin_, initialValue);
}

void LED::on() {
  pattern_ = 0xFFFF;
  digitalWrite(pin_, HIGH);
}

void LED::off() {
  pattern_ = 0x0000;
  digitalWrite(pin_, LOW);
}

void LED::blink(unsigned int period, unsigned int pattern) {
  period_ = period;
  pulse_width_ = ceil_div(period, 16);
  pattern_ = pattern;
}

void LED::loop(long currentMillis) {
  // First, find which bit of the pattern we should display.
  unsigned int bit = (currentMillis % period_) / pulse_width_;
  // Then find the value of the bit in the pattern.
  unsigned int value = bitRead(pattern_, bit);
  digitalWrite(pin_, value);
}