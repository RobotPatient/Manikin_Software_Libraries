#ifndef HAL_LOG_HPP
#define HAL_LOG_HPP
#include <Adafruit_SPIFlash.h>

class LogTransport {
public:
LogTransport(){

}

};

class LogTransportFlash : public LogTransport {
 public:
  LogTransportFlash() {}


};

class Logger {

};

#endif