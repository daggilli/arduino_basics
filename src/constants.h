#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#include <Arduino.h>

constexpr int SWITCH = 2;
constexpr int TOGGLE_LED = 7;
constexpr int PWM_LED = 9;
constexpr int POT = A5;
constexpr int TMP36 = A1;
constexpr unsigned long BAUD_RATE = 115200;
constexpr char LF = '\n';
constexpr size_t TX_BUF_SIZE = 32;
constexpr size_t RX_BUF_SIZE = 32;
constexpr size_t FP_BUF_SIZE = 16;
constexpr size_t MOV_AVG_LEN = 32;
#endif
