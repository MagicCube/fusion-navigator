#pragma once

#include <Arduino.h>

#include <Encoder.h>

#include "Joystick.h"

class FusionNavigator {
public:
  FusionNavigator(Joystick *joystick, Encoder *encoder);

  void begin();
  void update();

private:
  Joystick *_joystick = NULL;
  Encoder *_encoder = NULL;

  int32_t _encoderValue = 0;

  unsigned long _lastUpdateTime = 0;
  uint8_t _updateInterval = 50;
};
