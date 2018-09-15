#include "FusionNavigator.h"

#include <HID-Project.h>

const uint8_t JOYSTICK_FACTOR = 28;
const uint8_t SCROLL_DIFFICULTY_WHEN_MOVING = 5;

FusionNavigator::FusionNavigator(Joystick *joystick, Encoder *encoder, uint8_t shiftButtonPin) {
  _joystick = joystick;
  _encoder = encoder;
  _shiftButtonPin = shiftButtonPin;
}

void FusionNavigator::begin() {
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
    int32_t encoderValue = _encoder->read();
    int delta = 0;
    if (_encoderValue != encoderValue) {
      delta = encoderValue - _encoderValue;
      _encoderValue = encoderValue;
    }
    if (_joystick->isActive()) {
      if (abs(delta) <= SCROLL_DIFFICULTY_WHEN_MOVING) {
        delta = 0;
      }
    }
    if (delta != 0 || _joystick->isActive()) {
      if (!_active && _joystick->isActive()) {
        // NOTE: The value of shift key is inversed.
        if (digitalRead(_shiftButtonPin) == HIGH) {
          Keyboard.press(HID_KEYBOARD_LEFT_SHIFT);
        }
      }

      _active = true;
      // Send changes to HID if the joystick or wheel moved.
      auto x = _joystick->x() / JOYSTICK_FACTOR;
      auto y = _joystick->y() / JOYSTICK_FACTOR;

      // [Mouse]
      if (_joystick->isActive()) {
        // Press the middle button
        Mouse.press('\010');
      } else {
        // Release when scrolling only
        Mouse.releaseAll();
      }
      Mouse.move(x, y, delta);
    } else {
      if (_active) {
        _releaseAll();
      }
      _active = false;
    }
    _lastUpdateTime = millis();
  }
}

void FusionNavigator::_releaseAll() {
  Mouse.releaseAll();
  Keyboard.releaseAll();
}
