#ifndef Encoder_h
#define Encoder_h
#include "Arduino.h"
class Encoder {
  public:
    Encoder(int pin_a, int pin_b, int enc_n = 36);
    int getRPM();
    int getEncoderCount();
    int setMaxMeasurements();
  private:
    int _pin_a;
    int _pin_b;
    int _enc_n;
    int _avg_rpm = 0;
    bool _meas_done = 0;
    int _meas_total = 0;
    int _meas_quant = 10;
    int _meas_counter = 0;
    unsigned long _t1, _t2, _t;
    long _enc_count = 0;
    int _enc_dir = 0;
    void _INTO_ISR();

};
#endif
