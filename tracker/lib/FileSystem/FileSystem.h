#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <SdFat.h>
#include <RTClib.h>

/**
 * An abstract class which knows how to log itself to a CSV file.
 */
class Record {
public:
  virtual void writeHeader(Print& output) = 0;
  virtual void write(Print& output) = 0;
};

class FileSystem {
  private:
    SdSpiConfig config_;
    SdFat32 sd_;
    File32 dataFile_;
  
  public:
    FileSystem(SdSpiConfig& config) : config_{config} {}
    bool begin(Record& record);
    void write(Record& record);
};

#endif