#include "include/PHSensor.h"

PHSensor::PHSensor(uint8_t pin, float calibration_value)
  : _pin(pin), _calibration(calibration_value) {
  pinMode(_pin, INPUT);
}

float PHSensor::readPH() {
  int temp;
  unsigned long avgval = 0;
  float ph_act = 0.0;

  // Coleta de amostras
  for (int i = 0; i < 10; i++) {
    _buffer[i] = analogRead(_pin);
    delay(30);
  }

  // Ordenação simples (selection)
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (_buffer[i] > _buffer[j]) {
        temp = _buffer[i];
        _buffer[i] = _buffer[j];
        _buffer[j] = temp;
      }
    }
  }

  // Média dos valores centrais
  avgval = 0;
  for (int i = 2; i < 8; i++) {
    avgval += _buffer[i];
  }

  float volt = (float)avgval * 5.0 / 1024.0 / 6.0;
  ph_act = -5.70 * volt + _calibration;

  return ph_act;
}
