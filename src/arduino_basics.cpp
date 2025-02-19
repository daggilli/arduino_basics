#include "arduino_basics.h"

#include <Arduino.h>
#include <string.h>

#include "constants.h"
#include "movingAverage.h"

char txbuf[TX_BUF_SIZE];
char rxbuf[RX_BUF_SIZE];
char fpbuf[FP_BUF_SIZE];

MovingAverage<int> potMovingAverage;
MovingAverage<double> tempMovingAverage;

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWITCH, INPUT);
  pinMode(TOGGLE_LED, OUTPUT);
  pinMode(PWM_LED, OUTPUT);
  digitalWrite(TOGGLE_LED, LOW);
  struct Sensors s = readSensors();
  potMovingAverage.fill(s.potValue);
  tempMovingAverage.fill(s.temp);
}

void loop() {
  checkButtonState();

  if (receiveLine()) {
    if (isValidMessage()) {
      toggleLedState();
    }
    clearReceiveBuffer();
  };
}

void checkButtonState() {
  static uint8_t lastState = LOW;

  struct Sensors s = readSensors();
  potMovingAverage.addReading(s.potValue);
  tempMovingAverage.addReading(s.temp);
  uint8_t buttonState = digitalRead(SWITCH);
  if (lastState != buttonState) {
    digitalWrite(LED_BUILTIN, buttonState);

    int brightness = rampBrightness(buttonState);

    dtostrf(tempMovingAverage.meanValue(), 4, 1, fpbuf);
    snprintf(txbuf, TX_BUF_SIZE, "C:%d:%d:%d:%d:%s\n", lastState, buttonState,
             potMovingAverage.meanValue(), brightness, fpbuf);
    Serial.print(txbuf);
    lastState = buttonState;
  }
}

bool receiveLine() {
  static size_t idx = 0;
  char rcvd;
  bool gotLine = false;

  while (Serial.available() > 0) {
    rcvd = Serial.read();
    if (rcvd == LF) {
      rxbuf[idx] = '\0';
      gotLine = true;
      break;
    }

    rxbuf[idx++] = rcvd;
    if (idx >= RX_BUF_SIZE) {
      clearReceiveBuffer();
      idx = 0;
      break;
    }
  }

  if (gotLine) {
    gotLine = false;
    idx = 0;
    return true;
  }

  return false;
}

bool isValidMessage() { return strcmp(rxbuf, "TOG") == 0; }

void toggleLedState() {
  static bool ledState = false;

  ledState = !ledState;
  digitalWrite(TOGGLE_LED, ledState ? HIGH : LOW);
}

void clearReceiveBuffer() { memset(rxbuf, 0, RX_BUF_SIZE); }

int rampBrightness(const uint8_t buttonState) {
  static int brightness = 0;
  static int dir = 5;

  if (buttonState == HIGH) {
    brightness += dir;
  }
  if (brightness >= 255) {
    brightness = 255;
    dir = -5;
  }
  if (brightness <= 0) {
    brightness = 0;
    dir = 5;
  }
  analogWrite(PWM_LED, brightness);
  return brightness;
}

struct Sensors readSensors() {
  int sensorValue = analogRead(TMP36);
  double temp = 125.0 * sensorValue / 256 - 50;
  struct Sensors s {
    analogRead(POT), temp
  };

  return s;
}