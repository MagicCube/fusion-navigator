#include "FusionNavigator.h"

#include <HID-Project.h>

const uint8_t JOYSTICK_FACTOR = 28;
const uint8_t SCROLL_DIFFICULTY_WHEN_MOVING = 5;

FusionNavigator::FusionNavigator(Joystick *joystick, Encoder *encoder, uint8_t shiftButtonPin,
                                 uint8_t button1Pin) {
  _joystick = joystick;
  _encoder = encoder;
  _shiftButtonPin = shiftButtonPin;
  _button1Pin = button1Pin;
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
  pinMode(_button1Pin, INPUT_PULLUP);
  Mouse.begin();
  Keyboard.begin();
}

void FusionNavigator::update() {
  if (millis() - _lastUpdateTime > _updateInterval) {
    // Update joystick
    _joystick->update();

    if (JOYSTICK_CALIBRATING) {
      _joystick->printDebug();
    }

    // Check if encoder has been changed.
    int delta = _updateEncoder();

    if (_joystick->isPulled()) {
      if (_state == FusionNavState::INACTIVE) {
        // Newly started movement detected
        if (digitalRead(_shiftButtonPin) == HIGH) {
          _state = FusionNavState::ORBITING;
          // Hold the Shift key
          Keyboard.press(HID_KEYBOARD_LEFT_SHIFT);
        } else {
          _state = FusionNavState::PANNING;
        }
        Mouse.press(MOUSE_MIDDLE);
      }
      uint8_t speed = _state == FusionNavState::PANNING ? PAN_SPEED : ORBIT_SPEED;
      int8_t x = _joystick->x() * speed / 512;
      int8_t y = _joystick->y() * speed / 512;

      // Since I don't like zooming while orbiting or moving,
      // I add some difficulties to trigger zooming.
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
      // Joystick is not pulled
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

    _updateButtons();

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
  return -delta;
}

void FusionNavigator::_updateButtons() {
  if (digitalRead(BUTTON_1_PIN) == LOW) {
    if (!_button1Pressed) {
      _button1Pressed = true;
      Keyboard.press(HID_KEYBOARD_LEFT_CONTROL);
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(HID_KEYBOARD_H_AND_H);
      Keyboard.release(HID_KEYBOARD_LEFT_CONTROL);
      Keyboard.release(HID_KEYBOARD_LEFT_ALT);
      Keyboard.release(HID_KEYBOARD_H_AND_H);
    }
  } else {
    if (_button1Pressed) {
      _button1Pressed = false;
    }
  }
}

void FusionNavigator::_deactive() {
  Mouse.releaseAll();
  Keyboard.releaseAll();
  _state = FusionNavState::INACTIVE;
}
