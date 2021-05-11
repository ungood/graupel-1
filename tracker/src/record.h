#ifndef RECORD_H
#define RECORD_H

#include <TinyGPS++.h>


template <typename T>
class TelemetryField : public Record {
private:
  const char* name_;
  T value_;
  bool set_;

  public:
    TelemetryField(const char* name) : name_{name}, set_{false} {}
    
    inline void set(T value) {
      value_ = value;
      set_ = true;
    }

    inline void writeHeader(Print& output) {
      output.print(name_);
      output.print(',');
    }

    inline void write(Print& output) {
      if(set_) {
        output.print(value_);
      } else {
        output.print('*');
      }

      output.print(',');
    }
};

class TelemetryRecord : public Record {


public:
  TelemetryField<unsigned long> millis{"millis"};
  TelemetryField<uint16_t> year{"year"};
  TelemetryField<uint8_t> month{"month"};
  TelemetryField<uint8_t> day{"day"};
  TelemetryField<uint8_t> hour{"hour"};
  TelemetryField<uint8_t> minute{"minute"};
  TelemetryField<uint8_t> second{"second"};

  TelemetryField<double> voltage{"voltage"};

  TelemetryField<uint32_t> satellites{"satellites"};
  TelemetryField<double> gps_altitude_m{"gps_altitude_m"};
  TelemetryField<double> course{"course"};
  TelemetryField<double> speed_mps{"speed_mps"};
  TelemetryField<double> latitude{"latitude"};
  TelemetryField<double> longitude{"longitude"};

  TelemetryField<float> temperature_C{"temperature_C"};
  TelemetryField<float> pressure_Pa{"pressure_Pa"};
  TelemetryField<float> pressure_altitude_m{"pressure_altitude_m"};
  TelemetryField<float> humidity_RH{"humidity_RH"};

  void writeHeader(Print &output) {
    millis.writeHeader(output);
    
    year.writeHeader(output);
    month.writeHeader(output);
    day.writeHeader(output);
    
    hour.writeHeader(output);
    minute.writeHeader(output);
    second.writeHeader(output);

    voltage.writeHeader(output);
    
    satellites.writeHeader(output);
    gps_altitude_m.writeHeader(output);
    course.writeHeader(output);
    speed_mps.writeHeader(output);
    latitude.writeHeader(output);
    longitude.writeHeader(output);

    temperature_C.writeHeader(output);
    pressure_Pa.writeHeader(output);
    pressure_altitude_m.writeHeader(output);
    humidity_RH.writeHeader(output);
    
    output.println();
    output.flush();
  }

  void write(Print &output) { 
    millis.write(output);

    year.write(output);
    month.write(output);
    day.write(output);
    
    hour.write(output);
    minute.write(output);
    second.write(output);

    voltage.write(output);
    
    satellites.write(output);
    gps_altitude_m.write(output);
    course.write(output);
    speed_mps.write(output);
    latitude.write(output);
    longitude.write(output);

    temperature_C.write(output);
    pressure_Pa.write(output);
    pressure_altitude_m.write(output);
    humidity_RH.write(output);

    output.println();
    output.flush();
  }
};

#endif