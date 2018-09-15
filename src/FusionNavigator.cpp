#include "FusionNavigator.h"

#include <HID-Project.h>

FusionNavigator::FusionNavigator(Joystick *joystick, Encoder *encoder) {
  _joystick = joystick;
  _encoder = encoder;
}

void FusionNavigator::begin() {
  _lastUpdateTime = 0;
  _encoderValue = 0;
  _encoder->write(0);
  // _joystick->begin();
  Mouse.begin();
}

void FusionNavigator::update() {
  if (millis() - _lastUpdateTime > _updateInterval) {
    // Update joystick
    _joystick->update();
    // Check if encoder has been changed.
    int32_t encoderValue = _encoder->read();
    int8_t wheel = 0;
    if (_encoderValue != encoderValue) {
      wheel = encoderValue - _encoderValue;
      _encoderValue = encoderValue;
    }
    if (wheel != 0) {
      // Send changes to HID
      Mouse.move(0, 0, wheel);
      Serial.println(wheel);
    }
    _lastUpdateTime = millis();
  }
}
