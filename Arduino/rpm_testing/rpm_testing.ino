#include <Servo.h>
#include <QuadratureEncoder.h>
#include <TimerOne.h>

Servo motors[4];

const int pins[] = {11, 9, 10, 5};
const int forward_factor[] = {1, 1, -1, -1};
const int directions[] = {1, -1, -1, 1};
const int base_speed = 1500;
const int max_speed_time = 600;
const float neutral_speed_factor = 0.4;
const float full_speed_complement = 1 - neutral_speed_factor;

unsigned long last_millis = 0;
const unsigned int encoder_rate_us = 100;
const float encoder_to_rpm_factor = (1/(encoder_rate_us*0.000001)) * 60;

float serialCommands[3]:
float normalized_axes[2];


#define ENCODER_PIN 2
#define ENCODER_N   36

unsigned long T1 = 0, T2 = 0, T;

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
  };
  Serial.begin(115200);
  pinMode(ENCODER_PIN, INPUT);
  attachInterrupt(digitalPinToIterrupt(ENCODER_PIN), INTO_ISR, RISING);
}

void loop() {
    frec_htz = 1000000 / T;
    Motor_RPM = 60*frec_htz / ENCODER_N;
    if(Serial.available() > 0) {
      SerialCommands[0] = Serial.parseFloat();
      Serial.print("Valor esperado: ");
      Serial.print(mapearValor(SerialCommands[0]))
      Serial.print("RPM: ");
      Serial.println(Motor_RPM);
      delay(100);
} 

int mapearValor(float valor) { 
    float inMin = -1.0; 
    float inMax = 1.0; 
    float outMin = -152.0; 
    float outMax = 150.0; 
    float salida = (valor - inMin) * (outMax - outMin) / (inMax - inMin) + outMin; 
    return round(salida);
}
  

