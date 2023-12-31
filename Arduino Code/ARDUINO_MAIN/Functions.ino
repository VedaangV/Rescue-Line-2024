#include "Header.h"


//GREENNNNNNNNN
//General purpose functions for the entire program
volatile int enc = 0;
void setMultipleMotors(int left, int right) { //set motors
//int buff = right;
//right = left;
//left = buff;
  int boost = 0;//in case motors are slow
  //port 1
  left = left > 255 ? 255 : left;
  left = left < -255 ? -255 : left;
  
  if (left > 0)
  {

    digitalWrite(35, LOW);
    delayMicroseconds(5);
    digitalWrite(34, HIGH);
    analogWrite(12, left); //pwm pin 12
  }
  else if (left < 0)
  {
    #ifdef main_bot
    left -= boost;
    #endif
    digitalWrite(34, LOW);
    delayMicroseconds(5);
    digitalWrite(35, HIGH);
    analogWrite(12, -left);
  }
  else
  {
    digitalWrite(34, LOW);
    digitalWrite(35, LOW);
    analogWrite(12, 0);
  }

  right = left > 255 ? 255 : right;
  right = right < -255 ? -255 : right;
  right = -right;
  if (right > 0)
  {
    #ifdef main_bot
    right += boost;
    #endif
    digitalWrite(36, LOW);
    delayMicroseconds(5);
    digitalWrite(37, HIGH);
    analogWrite(8, right);
  }
  else if (right < 0)
  {
    #ifdef main_bot
    right -= boost;
    #endif
    digitalWrite(37, LOW);
    delayMicroseconds(5);
    digitalWrite(36, HIGH);
    analogWrite(8, -right);
  }
  else
  {
    digitalWrite(36, LOW);
    digitalWrite(37, LOW);
    analogWrite(8, 0);
  }

}

float cm_to_encoders(float cm) {
  const float wheelDiameter = 6.42;
  return cm / (wheelDiameter * PI) * 360;
}

void Interruptfunc() {//interrupt func for encoders
  if (digitalRead(31) == HIGH) {
    enc++;
  }
  else {
    enc--;
  }
}

void forward_enc(int encoders, int motor_speed) {//go forward for enc
  int previousEnc = enc;
  while (enc <=  previousEnc + encoders) {
    setMultipleMotors(motor_speed, motor_speed);
    Serial.println(enc);
  }
  setMultipleMotors(0, 0);
}
void backward_enc(int encoders, int motor_speed) {//go backward for enc
  int previousEnc = enc;

  while (enc >=  previousEnc - encoders) {
    setMultipleMotors(-motor_speed, -motor_speed);
  }
  setMultipleMotors(0, 0);
}
void backwardCm(float dist, int motor_speed) { //go backward for cm
  backward_enc(cm_to_encoders(dist), motor_speed);
}
void forwardCm(float dist, int motor_speed) {//go forward for cm
  forward_enc(cm_to_encoders(dist), motor_speed);
}

int getnum(char *p)
{
  int sum = 0;
  for(int i = 0; i < 3; i++)
  {
    sum *= 10;
    sum += (*(p+i) - '0');
  }
  return sum;
}