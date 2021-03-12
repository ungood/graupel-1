#ifndef BME280_H
#define BMD280_H

#include <Adafruit_BME280.h>

#define SEA_LEVEL_PRESSURE_HPA 1013.25

class Sensors {
    private:
        Adafruit_BME280 bme;
        Adafruit_Sensor *temperature = bme.getTemperatureSensor();
        Adafruit_Sensor *pressure = bme.getPressureSensor();
        Adafruit_Sensor *humidity = bme.getHumiditySensor();

    public:
        bool begin() {
            // TODO: Logging!

            return bme.begin(BME280_ADDRESS_ALTERNATE);
        }
    
        void loop() {
            Serial.print("Temperature: ");
            Serial.print(bme.readTemperature());

            Serial.print(", Humidity: ");
            Serial.print(bme.readHumidity());

            Serial.print(", Pressure: ");
            Serial.print(bme.readPressure());

            // TODO: Improve this for high altitude: https://forums.adafruit.com/viewtopic.php?f=22&t=58064#p531398
            Serial.print(", Altitude: ");
            Serial.print(bme.readAltitude(SEA_LEVEL_PRESSURE_HPA));

            Serial.println();
        }
};

#endif