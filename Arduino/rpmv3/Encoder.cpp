#include "Arduino.h"
#include "Encoder.h"

Encoder::Encoder(int pin_a, int pin_b, int enc_n) {
  pinMode(pin_a, INPUT);
  pinMode(pin_b, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin_a), _INTO_ISR, RISING);
  _pin_a = pin_a;
  _pin_b = pin_b;
  _enc_n = enc_n;
}

void Encoder::_INTO_ISR() {
  /*
    Measures time difference between interruptions and counts encoder pulses.
  */

  if (_meas_done) {
    _t2 = micros();
    _t = _t2 - _t1;
    _meas_done = 0;
    int rpm = 60 * (1000000 / _t) / _enc_n;
    _meas_total += rpm;
    if (_meas_counter == _meas_quant) {
      _avg_rpm = _meas_total / _meas_quant;
      _meas_counter = 0;
    } else {
      _meas_counter++;
    }

  } else {
    _t1 = micros();
    _meas_done = 1;
    if (digitalRead(_pin_a) == digitalRead(_pin_b)) {
      _enc_dir = 1;
      _enc_count++;
    } else {
      _enc_count--;
      _enc_dir = -1;
    }
  }
}

int Encoder::getEncoderCount() { return _enc_count; }

int Encoder::getRPM() { return _avg_rpm; }
