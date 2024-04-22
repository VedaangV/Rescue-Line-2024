#include "Header.h"
volatile long pulseMsSt = 0;
volatile long pulseMsEd = 0;
volatile bool pulseFirst = 1;
int trig = 4;
int echo = 5;
void distanceISR()
{

  if (pulseFirst)
  {
    pulseMsSt = micros();
    pulseFirst = 0;
  }
  else
  {
    pulseMsEd = micros();
    pulseFirst = 1;
  }
}

void forward_until_obs(int dist){
  Serial.println("FOBS STARTED");
  Serial.println(dist);
  while((int)getFrontDistance() > dist){
    setMultipleMotors(60, 60);
  }
  setMultipleMotors(0, 0);
}


float getFrontDistance()//get distance of Front US
{

  digitalWrite(trig, LOW);
  delay(1);
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  long pulseTime = pulseIn(echo, HIGH);
  float distance;
  distance = pulseTime * 0.034 / 2;
  if (distance < 0)
  {
    distance = 0;
  }
  return (distance);
}

bool seeObs(long dist) { //sees obstacle?
  if (getFrontDistance() <= dist) {
    return true;
  }
  else {
    return false;
  }
}