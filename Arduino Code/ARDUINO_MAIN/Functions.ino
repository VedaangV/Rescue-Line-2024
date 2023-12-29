#include "Header.h"


//GREENNNNNNNNN
//General purpose functions for the entire program

void setMultipleMotors(int left, int right) { //set motors
//int buff = right;
//right = left;
//left = buff;
left *= -1;
right *= -1;
  int boost = 0;//in case motors are slow
  //port 1
  left = left > 255 ? 255 : left;
  left = left < -255 ? -255 : left;
  
  if (left > 0)
  {
    #ifdef main_bot
    left += boost;
    #endif
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

int getnum(char *p)
{
  int sum = 0;
  for(int i = 0; i < 3; i++)
  {
    sum * 10;
    sum += (p - '0');
    p++;
  }
  return sum;
}