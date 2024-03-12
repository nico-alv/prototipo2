#include <Servo.h>
#include <QuadratureEncoder.h>
#include <TimerOne.h>

Servo motors[4];

const float vehicle_width = 0.5;


//Serial comm
const int expectedDataSize = 3;
const int bufferSize = 64;
char inputBuffer[bufferSize];

//Pin mappinga
//		   LU,LD,RU,RD 
const int pins[] = {11,9,10,5};
const int forwardFactor[] = {1,1,-1,-1};
const int directions[] = {1,-1,-1,1};

//Speed
const int baseSpeedTime = 1500;
const int maxSpeedTime = 600;
const float neutralSpeedFactor = 0.4;
const float fullSpeedComplement = (1-neutralSpeedFactor);

//Data storage
float serialCommands[expectedDataSize];
float normalized_axes[2];


unsigned long last_millis = 0;
const unsigned int encoder_rate_us = 100;
const float encoder_to_rpm_factor = (1/(encoder_rate_us*0.000001)) *  60;
/*
Encoders encoder0(A14 , A15);
Encoders encoder1(A12 , A13);
Encoders encoder2(A10 , A11);
Encoders encoder3(A8 , A9);
int encoderCounts[4];
*/
void setup() {
  for (int i = 0; i < 4; i++) {
      motors[i].attach(pins[i]);  
  }
  // put your setup code here, to run once:
  Serial.begin(115200);
  // Set up Timer1 with a period of 1000000 microseconds (1 second)
//  Timer1.initialize(10000);

  // // Attach the blink function to the timer interrupt
//  Timer1.attachInterrupt(EncoderCallback);

}

void loop() {
  // put your main code here, to run repeatedly:
 if (Serial.available() > 0) {
    Serial.println("here");
    int dataCount = 0;
    int bytesRead = Serial.readBytesUntil('\n', inputBuffer, bufferSize - 1);
    // Null-terminate the string
    inputBuffer[bytesRead] = '\0';
    char* command = strtok(inputBuffer, ",");
    while (command != 0 && dataCount <= expectedDataSize)
    {
        float parsedNumber = atof(command);
        serialCommands[dataCount] = parsedNumber;
        // Find the next command in input string
        command = strtok(0, ",");
        dataCount++;
    }
     Serial.println(serialCommands[0]);
     Serial.println(serialCommands[1]);
     Serial.println(serialCommands[2]);
     Serial.println("---------");

    if(dataCount != expectedDataSize) return;
  }
  MotorControl();
/*
  encoderCounts[0] = encoder0.getEncoderCount();
  encoderCounts[1] = encoder1.getEncoderCount();
  encoderCounts[2] = encoder2.getEncoderCount();
  encoderCounts[3] = encoder3.getEncoderCount();
*/
}

void MotorControl(){
  // NormalizeAxes(serialCommands[0], serialCommands[1]);
  float direction_multiplier = vehicle_width * (serialCommands[0]);
  float speed_factor = (neutralSpeedFactor + fullSpeedComplement * serialCommands[2])*maxSpeedTime;
  for (int i = 0; i < 4; i++) {
      float speed = serialCommands[1] + (direction_multiplier * directions[i]);
      int calc_speed = baseSpeedTime + (speed * forwardFactor[i] * speed_factor);
      motors[i].writeMicroseconds(calc_speed);
      // Serial.print(speed);
      // Serial.print(" ");
    }
    // Serial.print(speedL);
    // Serial.print(" ");
    // Serial.print(speedR);
    // Serial.println();
}
/*
void EncoderCallback(){
    Serial.println(encoder0.getEncoderCount());
    Serial.print(" ");
    Serial.print(encoder1.getEncoderCount());
    Serial.print(" ");
    Serial.print(encoder2.getEncoderCount());
    Serial.print(" ");
    Serial.print(encoder3.getEncoderCount());
    Serial.println();
}

*/

void NormalizeAxes(float x, float y){
  float mag =x*x + y*y;
  if (mag > 1) {
    float invertedMag = invSqrt(mag);
    normalized_axes[0] = x*invertedMag;
    normalized_axes[1] = y*invertedMag;
  } else {
    normalized_axes[0] = x;
    normalized_axes[1] = y;
  }
}


float invSqrt (float x){ 
  union{  
    int32_t i;  
    float   f; 
  } conv; 
  conv.f = x; 
  conv.i = 0x5f3759df - (conv.i >> 1); 
  return 0.5f * conv.f * (3.0f - x * conv.f * conv.f);
}
