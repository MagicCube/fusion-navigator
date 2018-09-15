#include "Joystick.h"

const uint8_t MIN_CHANGE = 2;

Joystick::Joystick(uint8_t xPin, uint8_t yPin) {
  _xPin = xPin;
  _yPin = yPin;
}

int Joystick::x() {
  return _x;
}

int Joystick::y() {
  return _y;
}

bool Joystick::isActive() {
  return _x != 0 || _y != 0;
}

void Joystick::begin() {
  _x = 0;
  _y = 0;
  _lastUpdateTime = 0;
}

void Joystick::update() {
  if (millis() - _lastUpdateTime > _updateInterval) {
    int x = analogRead(_xPin) - 512 - _adjustmentX;
    int y = analogRead(_yPin) - 512 - _adjustmentY;

    if (abs(x) <= MIN_CHANGE) {
      x = 0;
    }
    if (abs(y) <= MIN_CHANGE) {
      y = 0;
    }

    if (x > 511) {
      x = 511;
    }
    if (x < -511) {
      x = -511;
    }

    if (y > 511) {
      y = 511;
    }
    if (y < -511) {
      y = -511;
    }

    if (_x != x || _y != y) {
      _x = x;
      _y = y;
    }

    printDebug();

    _lastUpdateTime = millis();
  }
}

void Joystick::printDebug() {
  Serial.print(_x);
  Serial.print(", ");
  Serial.print(_y);
  Serial.println();
}
