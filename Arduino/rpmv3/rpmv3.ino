#include <Servo.h>

// Control variables
Servo motors[4];
                // LU, RD, LD, RU
const int pins[] =  {11, 9, 10, 5};
const float kp[]   =  {0.01, 0, 0, 0};
const float ki[]   =  {0, 0, 0, 0};
const float kd[]   =  {0, 0, 0, 0};

int curr_pwm[] = {0, 0, 0, 0};
int correction[] = {0, 0, 0, 0};
int meas_done[] = {0, 0, 0, 0};

#define ENC_N 36;
int rpm[] = {0, 0, 0, 0};
int enc_pin_a = {2, 0, 0, 0};
int enc_pin_b = {3, 0, 0, 0};
int err[] = {0, 0, 0, 0};
int prev_err[] = {0, 0, 0, 0};
int microseconds[] = {0, 0, 0, 0};

// Movement modifiers
const float vehicle_width = 0.5;
const int forward_factor[] = {1, 1, -1, -1};
const int directions[]     = {1, -1, -1, 1};
const int base_speed = 1500;
const int max_speed_time = 600;
const float neutral_speed_factor = 0.4;
const float full_speed_complement = 1 - neutral_speed_factor;
// Serial parsing
const int expectedDataSize = 3;
float serialCommands[expectedDataSize];
// Encoder time measurement
#define ENCODER_N 36
int meas_quant = 10;
int meas_total[0] = {0, 0, 0, 0};
int meas_counter = 0;
unsigned long last_millis = 0;
unsigned long t1[] = {0, 0, 0, 0}, t2[] = {0, 0, 0, 0}, t[] = {0, 0, 0, 0};
long enc_count[] = {0, 0, 0, 0};
int enc_dir[] = {0, 0, 0, 0};
bool meas_done[] = {0, 0, 0, 0};

void isr()
{
  if (meas_done[0])
  {
    t2[0] = micros();
    t[0] = t2[0] - t1[0];
    meas_done[0] = 0;
  }
  else
  {
    t1[0] = micros();
    meas_done[0] = 1;
    if(digitalRead(enc_pin_a[0]) == digitalRead(enc_pin_b[0])){
      enc_dir[0] = 1;
      enc_count[0]++;
    }
    else {
    enc_count[0]--;
    enc_dir[0] = -1;
    }
  }
}

void isr2()
{
  if (meas_done)
  {
    t2[1] = micros();
    t[1] = t2[1] - t1[1];
    meas_done[1] = 0;
  }
  else
  {
    t1[1] = micros();
    meas_done[1] = 1;
    if(digitalRead(enc_pin_a[1]) == digitalRead(enc_pin_b[1])){
      enc_dir[1] = 1;
      enc_count[1]++;
    }
    else {
    enc_count[1]--;
    enc_dir[1] = -1;
    }
  }
}

void isr3()
{
  if (meas_done[2])
  {
    t2[2] = micros();
    t[2] = t2[2] - t1[2];
    meas_done[2] = 0;
  }
  else
  {
    T1[2] = micros();
    meas_done[2] = 1;
    if(digitalRead(enc_pin_a[2]) == digitalRead(enc_pin_b[2])){
      enc_dir[2] = 1;
      enc_count[2]++;
    }
    else {
    enc_count[2]--;
    enc_dir[2] = -1;
    }
  }
}

void isr4()
{
  if (meas_done[3])
  {
    t2[3] = micros();
    t[3] = t2[3] - t1[3];
    meas_done[3] = 0;
  }
  else
  {
    t1[3] = micros();
    meas_done[3] = 1;
    if(digitalRead(enc_pin_a[3]) == digitalRead(enc_pin_b[3])){
      enc_dir[3] = 1;
      enc_count[3]++;
    }
    else {
    enc_count[3]--;
    enc_dir[3] = -1;
    }
  }

}

void setup()
{
  for (int i = 0; i < 4; i++)
  {
    motors[i].attach(pins[i]);
  }

  Serial.begin(115200);
  for(int i=0; i<sizeof(enc_pin_a); i++){
    pinMode(enc_pin_a[i], INPUT);
    pinMode(enc_pin_b[i], INPUT);
  }

  serialCommands[0] = 0;
  serialCommands[1] = 0.5;
  serialCommands[2] = 1;
  attachInterrupt(digitalPinToInterrupt(enc_pin_a[0]), isr, RISING);
  attachInterrupt(digitalPinToInterrupt(enc_pin_a[1]), isr2, RISING);
  attachInterrupt(digitalPinToInterrupt(enc_pin_a[2]), isr3, RISING);
  attachInterrupt(digitalPinToInterrupt(enc_pin_a[3]), isr4, RISING);
}

void loop()
{
  for(int i=0;i<sizeof(enc_pin_a);i++){
    float freq = 1000000 / t[i];
    rpm[i] = 60 * freq / ENC_N;
    meas_total[i] += rpm[i];
    if(meas_counter == meas_quant){
      avg_rpm[i] = meas_total[i] / meas_quant;
      meas_counter = 0;
    } else {
      meas_counter++;
    }
    microseconds[i] = (avg_rpm[i] * 2100/150);
    prev_err[i] = err[i];
    err[i] = curr_pwm[0] - microseconds[0];
  }
  Serial.print("RPM:");
  Serial.print(avg_rpm[0] * enc_dir[0]);
  Serial.print(" (");
  Serial.print(microseconds[0]);
  Serial.print("), PWM:");
  Serial.print(curr_pwm[0]);
  Serial.print(", err:");
  Serial.print(err[0]);
  Serial.print(", correction:");
  Serial.println(correction[0]);
  if (Serial.available() > 0 ){
    serialCommands[1] = Serial.parseFloat(); 
    Serial.println(serialCommands[1]);
  }
  MotorControl();
}

void MotorControl()
{
  float direction_multiplier = vehicle_width * serialCommands[0]; //hor. movment
  float speed_factor = (neutral_speed_factor + full_speed_complement * serialCommands[2]) * max_speed_time; //turbo
  
  for (int i = 0; i < 4; i++)
  {
    float speed = serialCommands[1] + (direction_multiplier * directions[i]); 
    int pwm = base_speed + (speed * forward_factor[i] * speed_factor);
    correction[i] = kp[i] * err[i] + ki[i]*(err[i]+prev_err[i]) + kd[i]*(err[i]-prev_err[i]); 
    motors[i].writeMicroseconds(pwm + correction[i]);
    curr_pwm[i] = pwm + correction[i];
    //Serial.print("kp: ");
    //Serial.print(kp[i]);
    //Serial.println(err[i]);
  }
}

int expected_rpm(float value) {
  float inMin = -1.0;
  float inMax = 1.0;
  float outMin = -152.0;
  float outMax = 150.0;
  float output = (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin; 
  return round(output);
}
