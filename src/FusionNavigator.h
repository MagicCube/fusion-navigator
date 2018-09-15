#pragma once

#include <Arduino.h>

#include <Encoder.h>

#include "Joystick.h"

class FusionNavigator {
public:
  FusionNavigator(Joystick *joystick, Encoder *encoder, uint8_t shiftPin);

  void begin();
  void update();

  void printDebug();

private:
  void _releaseAll();

  bool _active = false;

  Joystick *_joystick = NULL;
  Encoder *_encoder = NULL;
  uint8_t _shiftButtonPin = 0;

  int32_t _encoderValue = 0;

  unsigned long _lastUpdateTime = 0;
  uint8_t _updateInterval = 50;
};
