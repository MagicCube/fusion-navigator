#pragma once

#include <Arduino.h>

// A class represents HW-504 joystick module.
class Joystick {
public:
  // Creates a new instance of class `Joystick` by passing X and Y pins.
  //
  // NOTE: In most cases, HW-504 needs a calibration, that's why you need `offsetX` and `offsetY`.
  //       Use `printDebug()` to print out the offsets to serial monitor, and please do not pull the
  //       joystick in order to get the right offsets.
  Joystick(uint8_t xPin, uint8_t yPin, int offsetX = 0, int offsetY = 0);

  // Gets `X` value of the joystick.
  //   Max value:  511
  //   Min value: -511
  //     Default:    0
  int x();

  // Gets `y` value of the joystick.
  //   Max value:  511
  //   Min value: -511
  //     Default:    0
  int y();

  // Gets a boolean indicates whether the joystick has been pulled.
  bool isPulled();

  // Initialize the joystcik.
  void begin();

  // Invokes inside `loop()`.
  void update();

  // Prints the current state of the joystick.
  void printDebug();

private:
  int _x = 0;
  int _y = 0;

  int8_t _offsetX = 0;
  int8_t _offsetY = 0;

  uint8_t _xPin = 0;
  uint8_t _yPin = 0;

  unsigned int _updateInterval = 80;
  unsigned long _lastUpdateTime = 0;
};
