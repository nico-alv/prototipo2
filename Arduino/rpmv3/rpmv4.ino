#include <Servo.h>
#include <Encoder.h>

// Control variables
Servo motors[4];
int err[4], prev_err[4];
                // LU, RD, LD, RU
const int pins[] =  {11, 9, 10, 5};
const float kp[]   =  {0.01, 0, 0, 0};
const float ki[]   =  {0, 0, 0, 0};
const float kd[]   =  {0, 0, 0, 0};

int curr_pwm[] = {0, 0, 0, 0};
int correction[] = {0, 0, 0, 0};

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
#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3
#define ENCODER_N 36
unsigned long last_millis = 0;

Encoder encoder1(2, 3);

void setup()
{
  for (int i = 0; i < 4; i++)
  {
    motors[i].attach(pins[i]);
  }

  Serial.begin(115200);
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  serialCommands[0] = 0;
  serialCommands[1] = 0.5;
  serialCommands[2] = 1;
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), INT0_ISR, RISING);
}

void loop()
{
  int microseconds = (rpm * 2100/150);
  //int microseconds = map(rpm*encoder_direction, -150, 150, 2100, 900);
  prev_err[0] = err[0];
  err[0] = curr_pwm[0] - microseconds;
  Serial.print("RPM:");
  Serial.print(rpm * encoder_direction);
  Serial.print(" (");
  Serial.print(microseconds);
  Serial.print("), PWM:");
  Serial.print(curr_pwm[0]);
  Serial.print(", err:");
  Serial.print(err[0]);
  Serial.print(", correction:");
  Serial.println(correction[0]);
  //Serial.print(" Command: ");
  //Serial.println(serialCommands[1]);
  if (Serial.available() > 0 ){
    serialCommands[1] = Serial.parseFloat(); 
    Serial.println(serialCommands[1]);
  }
  MotorControl();
}

void MotorControl()
{
  const float vehicle_width = 0.5;
  const int forward_factor[] = {1, 1, -1, -1};
  const int directions[] = {1, -1, -1, 1};
  const int base_speed = 1500;
  const int max_speed_time = 600;
  const float neutral_speed_factor = 0.4;
  const float full_speed_complement = 1 - neutral_speed_factor;

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
