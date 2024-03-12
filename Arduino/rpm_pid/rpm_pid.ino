#include <Servo.h>

// Motor data
Servo motors[4];
                // LU, RD, LD, RU
const int pins[] = {11, 9, 10, 5};
int rpm_motors[] = {0, 0, 0, 0};
// Movement modifiers
const float vehicle_width = 0.5;
const int forward_factor[] = {1, 1, -1, -1};
const int directions[] = {1, -1, -1, 1};
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
unsigned long T1 = 0, T2 = 0, T;
long encoder_count = 0;
int encoder_direction = 0;
bool MeasDone = 0;

void INT0_ISR(void)
{
  if (MeasDone)
  {
    T2 = micros();
    T = T2 - T1;
    MeasDone = 0;
  }
  else
  {
    T1 = micros();
    MeasDone = 1;
    if(digitalRead(ENCODER_PIN_A) == digitalRead(ENCODER_PIN_B)){
      encoder_direction = 1;
      encoder_count++;
    }
    else {
    encoder_count--;
    encoder_direction = -1;
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
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  serialCommands[0] = 0;
  serialCommands[1] = 0.5;
  serialCommands[2] = 1;
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), INT0_ISR, RISING);
}

void loop()
{
  float freq = 1000000 / T;
  int rpm = 60 * freq / ENCODER_N;
  //int microseconds = (rpm * encoder_direction * 2100/150);
  int microseconds = map(rpm*encoder_direction, -150, 150, 2100, 900);
  int err = rpm_motors[0] - microseconds;
  Serial.print(rpm * encoder_direction);
  Serial.print(" (");
  Serial.print(microseconds);
  Serial.print("), PWM:");
  Serial.print(rpm_motors[0]);
  Serial.print(", err: ");
  Serial.println(err);
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
  float direction_multiplier = vehicle_width * serialCommands[0];
  float speed_factor = (neutral_speed_factor + full_speed_complement * serialCommands[2]) * max_speed_time;
  for (int i = 0; i < 4; i++)
  {
    float speed = serialCommands[1] + (direction_multiplier * directions[i]);
    int calc_speed = base_speed + (speed * forward_factor[i] * speed_factor);
    motors[i].writeMicroseconds(calc_speed);
    rpm_motors[i] = calc_speed;
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
