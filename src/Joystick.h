#pragma once

#include <Arduino.h>

#include "settings.h"

class Joystick {
public:
  Joystick(uint8_t xPin, uint8_t yPin);

  int x();
  int y();
  bool isActive();

  void begin();
  void update();

  void printDebug();

private:
  int _x = 0;
  int _y = 0;

  int8_t _adjustX = 1;
  int8_t _adjustY = -5;

  uint8_t _xPin = 0;
  uint8_t _yPin = 0;

  unsigned int _updateInterval = UPDATE_INTERVAL;
  unsigned long _lastUpdateTime = 0;
};
