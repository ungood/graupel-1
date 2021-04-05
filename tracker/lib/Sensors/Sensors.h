#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <BME280.h>

const float SEA_LEVEL_PRESSURE_PA = 101325.0;

struct SensorReading {
  float temperature;
  float pressure;
  float altitude;
  float humidity;
};

class Sensors {
  private:
    BME280 bme_;

  public:
    Sensors(TwoWire& wire, uint8_t address) : bme_{wire, address} {}

    inline bool begin() {
      return bme_.begin() > 0;
    }
  
    float computeAltitude(float atmosphericPressurePa, float seaLevelPressurePa);
    bool read(SensorReading& reading);
};

#endif