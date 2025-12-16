#ifndef PHSENSOR_H
#define PHSENSOR_H

#include <Arduino.h>

class PHSensor {
  public:
    PHSensor(uint8_t pin, float calibration_value = 21.34 - 0.7);
    float readPH();

  private:
    uint8_t _pin;
    float _calibration;
    int _buffer[10];
};

#endif
