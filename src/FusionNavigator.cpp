#include "FusionNavigator.h"

#include <HID-Project.h>

const uint8_t JOYSTICK_FACTOR = 28;
const uint8_t SCROLL_DIFFICULTY_WHEN_MOVING = 5;

FusionNavigator::FusionNavigator(Joystick *joystick, Encoder *encoder, uint8_t shiftButtonPin) {
  _joystick = joystick;
  _encoder = encoder;
  _shiftButtonPin = shiftButtonPin;
}

FusionNavState FusionNavigator::state() {
  return _state;
}

void FusionNavigator::begin() {
  _state = FusionNavState::INACTIVE;
  _zooming = false;
  _lastUpdateTime = 0;
  _encoderValue = 0;
  _encoder->write(0);
  _joystick->begin();
  pinMode(_shiftButtonPin, INPUT_PULLUP);
  Mouse.begin();
  Keyboard.begin();
}

void FusionNavigator::update() {
  if (millis() - _lastUpdateTime > _updateInterval) {
    // Update joystick
    _joystick->update();

    // Check if encoder has been changed.
    int delta = _updateEncoder();

    if (_joystick->isActive()) {
      if (_state == FusionNavState::INACTIVE) {
        // Newly started movement detected
        if (digitalRead(_shiftButtonPin) == HIGH) {
          _state = FusionNavState::ORBITING;
          // Hold the Shift key
          Keyboard.press(HID_KEYBOARD_LEFT_SHIFT);
        } else {
          _state = FusionNavState::PANNING;
        }
        // Hold the middle button
        Mouse.press('\010');
      }
      uint8_t speed = _state == FusionNavState::PANNING ? PAN_SPEED : ORBIT_SPEED;
      int8_t x = _joystick->x() * speed / 512;
      int8_t y = _joystick->y() * speed / 512;

      if (delta) {
        if (!_zooming) {
          if (delta >= 2) {
            _zooming = true;
          }
        }
      } else {
        _zooming = false;
      }

      Mouse.move(x, y, _zooming ? delta : 0);
    } else {
      // Joystick is not active
      if (delta != 0) {
        // If zooming
        _zooming = true;
        Mouse.move(0, 0, delta);
      } else {
        _zooming = false;
        if (_state != FusionNavState::INACTIVE) {
          _deactive();
        }
      }
    }

    // Remember the current time
    _lastUpdateTime = millis();
  }
}

int FusionNavigator::_updateEncoder() {
  int32_t encoderValue = _encoder->read();
  int delta = 0;
  if (_encoderValue != encoderValue) {
    delta = encoderValue - _encoderValue;
    _encoderValue = encoderValue;
  }
  return delta;
}

void FusionNavigator::_deactive() {
  Mouse.releaseAll();
  Keyboard.releaseAll();
  _state = FusionNavState::INACTIVE;
}
