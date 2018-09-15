#pragma once

#include <Arduino.h>

#include <Encoder.h>

#include "settings.h"

#include "Joystick.h"

enum class FusionNavState { INACTIVE, PANNING, ORBITING };

class FusionNavigator {
public:
  FusionNavigator(Joystick *joystick, Encoder *encoder, uint8_t shiftPin);

  FusionNavState state();
  bool isZooming();

  void begin();
  void update();

  void printDebug();

private:
  int _updateEncoder();
  void _deactive();

  FusionNavState _state = FusionNavState::INACTIVE;
  bool _zooming = false;

  Joystick *_joystick = NULL;
  Encoder *_encoder = NULL;
  uint8_t _shiftButtonPin = 0;

  int32_t _encoderValue = 0;

  unsigned long _lastUpdateTime = 0;
  uint8_t _updateInterval = UPDATE_INTERVAL;
};
