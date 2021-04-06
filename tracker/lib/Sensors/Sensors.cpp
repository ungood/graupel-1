#include <Sensors.h>

float Sensors::computeAltitude(float atmosphericPressurePa, float seaLevelPressurePa) {
  // Equation taken from BMP180 datasheet (page 16):
  //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

  // Note that using the equation from wikipedia can give bad results
  // at high altitude. See this thread for more information:
  //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064
  return 44330.0 * (1.0 - pow(atmosphericPressurePa / seaLevelPressurePa, 0.1903));
}

bool Sensors::read(SensorReading& reading) {
  int status = bme_.readSensor();
  if(status < 0) {
    return false;
  }

  reading.temperature_C = bme_.getTemperature_C();
  reading.humidity_RH = bme_.getHumidity_RH();
  reading.pressure_Pa = bme_.getPressure_Pa();
  reading.altitude_m = computeAltitude(reading.pressure_Pa, SEA_LEVEL_PRESSURE_PA);  
  return true;
}