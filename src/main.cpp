#include <Arduino.h>

#include <Encoder.h>

#include "FusionNavigator.h"
#include "Joystick.h"


#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define ENCODER_CLS_PIN 2
#define ENCODER_DT_PIN 3
#define ENCODER_SW_PIN 4


Joystick joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN);
Encoder encoder(ENCODER_CLS_PIN, ENCODER_DT_PIN);
FusionNavigator fusionNavigator(&joystick, &encoder, ENCODER_SW_PIN);

void setup() {
  fusionNavigator.begin();
}

void loop() {
  fusionNavigator.update();
}
