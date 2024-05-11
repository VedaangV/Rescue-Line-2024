#include "Header.h"
volatile long pulseMsSt = 0;
volatile long pulseMsEd = 0;
volatile bool pulseFirst = 1;
int trig = 4;
int echo = 5;
void distanceISR() {

  if (pulseFirst) {
    pulseMsSt = micros();
    pulseFirst = 0;
  } else {
    pulseMsEd = micros();
    pulseFirst = 1;
  }
}

void forward_until_obs(int dist) {
  Serial.println("FOBS STARTED");
  Serial.println(dist);
  while ((int)getFrontDistance() > dist) {
    setMultipleMotors(60, 60);
  }
  setMultipleMotors(0, 0);
}


float getFrontDistance()  //get distance of Front US
{

  digitalWrite(trig, LOW);
  delay(1);
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  long pulseTime = pulseIn(echo, HIGH);
  float distance;
  distance = pulseTime * 0.034 / 2;
  if (distance < 0) {
    distance = 0;
  }
  return (distance);
}

bool seeObs(long dist) {  //sees obstacle?
  if (getFrontDistance() <= dist) {
    return true;
  } else {
    return false;
  }
}
void avoid(int sign) {  //move around obstacle
  bool flag = false;
  forwardCm(15.0, 100);
  enc_turn(-90 * sign, 100);
  forwardCm(40.0, 70);
  enc_turn(-90 * sign, 100);
  int enc1 = enc;
  while (leftBlack() == 0 && rightBlack() == 0) {      //forward until black line
    if (enc - enc1 >= cm_to_encoders(9.0) && !flag) {  //if traveled more than or 22cm (lost the line)
      enc_turn(-90 * sign, 100);                       //turn again to find the line
      flag = true;                                     //ensures that it only does this pnce
    }
    setMultipleMotors(70, 70);
    qtr.read(bw_vals);
  }
  forwardCm(15.0, 70);
  enc_turn(90 * sign, 100);
}
/*void avoid(int sign){
  float botLength = 15.5;
  float proportion = 5.0;
  float baseSpeed = 30.0;
  float obs_width = 10.0;//assumed width of obstacle for testing
  float bot_dist = 5.0;//the distance between the bot and obstacle before turning (measured)
  float circum = (((obs_width + (2 * bot_dist)) * PI)/2) - botLength/2;//calculation of the circumference of the circular path bot must take
  float previous_enc = enc;
  int inner_sensor;
  while(enc- previous_enc < 0.2 * (cm_to_encoders(circum))){//until completed at least 20% of the turn
    setMultipleMotors(baseSpeed + (baseSpeed * (proportion-1) * (sign < 0)), ((baseSpeed*proportion) -  (baseSpeed * (proportion-1) * (sign < 0))));//circular turn, speeds: (base_speed * proportion), base_speed
  }
  qtr.read(bw_vals);
  if(sign == 1){
    inner_sensor = leftSensor;
  }
  else{
    inner_sensor = rightSensor;
  }
  while(bw_vals[inner_sensor] < BLACK_THRESH && enc- previous_enc < (cm_to_encoders(circum))){//keep turning until black line
    qtr.read(bw_vals);
  }
  forwardCm(5.0, 80);
  
  qtr.read(bw_vals);
  while((leftBlack() == 0 && rightBlack() == 0)){//turn back onto line
    qtr.read(bw_vals);
    setMultipleMotors(100 * sign, -100*sign);
  }

 //turning back straight gradually
 
  forwardCm(0.7, 80);
  enc_turn(7.5 * sign, 120);
  forwardCm(0.7, 80);
  enc_turn(7.5 * sign, 120);
   
}*/

float getDistance() {
  int16_t t = pulseIn(2, HIGH);
  if (t == 0) {
    // pulseIn() did not detect the start of a pulse within 1 second.
    //Serial.println("timeout");
  } else if (t > 1850) {
    // No detection.
    //Serial.println(-1);
  } else {
    // Valid pulse width reading. Convert pulse width in microseconds to distance in centimeters.
    float d = (t - 1000) * 33.0 / 160.0;

    // Limit minimum distance to 0.
    if (d < 0) { d = 0; }
    return d;
  }
}
  void obstacle() {  //main obstacle function
    if (seeObs(5.0)) {
      enc_turn(90, 100);
      if (seeObs(10.0)) {  //sees wall
        enc_turn(180, 100);
        avoid(-1);
      } else {
        avoid(1);
      }
    }
    setMultipleMotors(0, 0);
  }