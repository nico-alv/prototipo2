#include <Servo.h>
#include <QuadratureEncoder.h>

Servo motors[4];
const int pins[] = {11, 9, 10, 5};
const int forward_factor[] = {1, 1, -1, -1};
const int directions[] = {1, -1, -1, 1};
const int base_speed = 1500;
const int max_speed_time = 600;
const float neutral_speed_factor = 0.4;
const float full_speed_complement = 1 - neutral_speed_factor;

unsigned long last_millis = 0;

float serialCommands[3];
float normalized_axes[2];

#define ENCODER_PIN_A 2
#define ENCODER_PIN_B 3
#define ENCODER_N 36

Encoders encoder(ENCODER_PIN_A, ENCODER_PIN_B);
long encoder_count = 0;
unsigned long encoder_time = 0;
int Motor_RPM = 0;

bool MeasDone = 0;
int Motor_RPM = 0;
float frec_htz = 0;

void INT0_ISR(void)
{
  if(MeasDone)
  {
    T2 = micros();
    T = T2 - T1;
    MeasDone = 0;
  }
  else
  {
    T1 = micros();
    MeasDone = 1;
  }
}


void setup() {
  for (int i = 0; i < 4; i++) {
    motors[i].attach(pins[i]);
  }

  Serial.begin(115200);
  pinMode(ENCODER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN), INTO_ISR, RISING);
}

void loop() {
  freq = 1000000 / T;
  rpm = 60*freq/ENCODER_N;
  Serial.print("RPM: ");
  Serial.println(rpm);

  
}

int mapearValor(float valor) {
  float inMin = -1.0;
  float inMax = 1.0;
  float outMin = -152.0;
  float outMax = 150.0;
  float salida = (valor - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
  return round(salida);
}

void MotorControl(){
  float direction_multiplier = vehicle_width * SerialCommands[0];
  float speed_factor = (neutral_speed_factor + full_speed_complement * serialCommands[2]) * max_speed_time;
  for (int i = 0; i < 4; i++){
    float speed = serialCommands[1] + (direction_multiplier * directions[i])
    int calc_speed = base_speed_time + (speed * forwardFactor[i] * speed_factor)
    motors[i].writeMicroseconds(calc_speed);
}
