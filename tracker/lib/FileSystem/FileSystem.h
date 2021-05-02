#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <SdFat.h>

class FileSystem {
  private:
    SdSpiConfig config_;
    SdFat32 sd_;
    File32 logFile_;
    File32 dataFile_;
    File32 stateFile_;
  
  public:
    FileSystem(SdSpiConfig& config) : config_{config} {}
    bool begin();
};

#endif