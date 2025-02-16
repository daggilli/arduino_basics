#ifndef __SERIALTEST_H__
#define __SERIALTEST_H__
#include <Arduino.h>

struct Sensors {
  int potValue;
  double temp;
};

void checkButtonState();
bool receiveLine();
bool isValidMessage();
void clearReceiveBuffer();
int rampBrightness(const uint8_t buttonState);
void toggleLedState();
struct Sensors readSensors();
#endif
