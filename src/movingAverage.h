#ifndef __MOVING_AVERAGE_H__
#define __MOVING_AVERAGE_H__
#include "constants.h"

template <typename T>
class MovingAverage {
 public:
  MovingAverage() : _idx(0), _sum(0) {}
  void fill(const T value) {
    for (size_t i = 0; i < MOV_AVG_LEN; i++) {
      _buffer[i] = value;
    }
    _sum = value * MOV_AVG_LEN;
  }
  void addReading(const T reading) {
    _sum -= _buffer[_idx];
    _sum += reading;
    _buffer[_idx++] = reading;
    _idx %= MOV_AVG_LEN;
  }
  const T meanValue() const { return _sum / MOV_AVG_LEN; };

 private:
  T _buffer[MOV_AVG_LEN];
  size_t _idx;
  T _sum;
};
#endif
