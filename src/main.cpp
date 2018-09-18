#include <Arduino.h>

#include <Encoder.h>

#include "FusionNavigator.h"
#include "Joystick.h"

Joystick joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN, JOYSTICK_OFFSET_X, JOYSTICK_OFFSET_Y);
Encoder encoder(ENCODER_CLS_PIN, ENCODER_DT_PIN);
FusionNavigator fusionNavigator(&joystick, &encoder, ENCODER_SW_PIN, BUTTON_1_PIN);

void setup() {
  fusionNavigator.begin();
}

void loop() {
  fusionNavigator.update();
}
