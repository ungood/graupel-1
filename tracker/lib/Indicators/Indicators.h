#ifndef INDICATORS_H
#define INDICATORS_H

#include <Arduino.h>

class LED {
private:
  byte pin_;

  // Number of milliseconds that the blink pattern should last.
  unsigned int period_ = 960;

  // Number of milliseconds that a pulse will last = period_ / 16;
  unsigned int pulse_width_ = 60;

  // A binary represenation of the pattern to blink.  Some example:
  // 0x0000 = Off
  // 0x000F = 25% dutry cycle
  // 0x00FF = 50% duty cycle
  // 0x0FFF = 75% duty cycle
  // 0xFFFF = 1111 1111 = On
  unsigned int pattern_ = 0x0000;

public:
  LED(byte pin) : pin_{pin} {}
  void begin(bool initialValue = false);
  void loop(long currentMillis);

  void on();
  void off();
  void blink(unsigned int period, unsigned int pattern = 0x00FF);
};

class Indicators {
public:
  LED ok;    // Flashes when initalizng, solid when all systems initialized.
  LED tx;    // Flashes while acquiring GPS signal, solid while transmitting.
  LED error; // Off when all systems good. Flashing when major error.

  Indicators(byte ok_pin, byte tx_pin, byte error_pin) : ok{ok_pin}, tx{tx_pin}, error{error_pin} {}

  inline void begin() {
    ok.begin();
    tx.begin();
    error.begin();
  }

  inline void on() {
    ok.on();
    tx.on();
    error.on();
  }

  inline void off() {
    ok.off();
    tx.off();
    error.off();
  }

  inline void loop(long currentMillis) {
    ok.loop(currentMillis);
    tx.loop(currentMillis);
    error.loop(currentMillis);
  }
};

#endif