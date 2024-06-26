#include "Header.h"
//General purpose functions for the entire program
volatile int enc = 0;
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
Adafruit_BNO055 bno = Adafruit_BNO055(55);
float yaw = 0;
const int m1_forward = -75;
const int m2_forward = 75;
float starting_yaw;
sensors_event_t orientationData;
imu::Vector<3> rot;




float getYaw()  //get rotation of BNO
{
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  rot = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  return orientationData.orientation.x;
}

float getPitch()  //get tilt (pitch) of BNO
{
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  return orientationData.orientation.y;
}
float getRoll()  //get tilt (pitch) of BNO
{
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  return orientationData.orientation.z;
}
void enc_turn(int deg, int speed)  //turns a specific amount of degrees
{
  bool neg = false;
  if (deg < 0)
    neg = true;
  int target = 0;
  setMultipleMotors(0, 0);
  yaw = getYaw();
  target = yaw + deg;


  if (target < 0) {
    target += 360;
  }


  else if (target > 360) {
    target -= 360;
  }
  Serial.print("Degree: ");
  Serial.println(deg);


  Serial.print("target: ");
  Serial.println(target);
  int calc_speed = ((deg * -1) * speed) / abs(deg);
  Serial.print("\n\n\n\n\n\n\n\n\n\n\nRIGHT MOTOR: ");
  Serial.println(calc_speed);
  Serial.print("LEFT MOTOR: ");
  Serial.println(-calc_speed);


  while ((yaw > target + 2) || (yaw < target - 2)) {
    //Serial.println(yaw);
    setMultipleMotors(-calc_speed, calc_speed);
    yaw = getYaw();
  }


  setMultipleMotors(0, 0);

  if (deg < 0) {
    setMultipleMotors(90, -90);
    delay(50);
  } else {
    setMultipleMotors(-90, 90);
    delay(50);
  }
}


void enc_turn_abs(int deg, int speed)  //turns a certain amount of degrees, rounding starting rotation to the nearest 90
{
  bool neg = false;
  int target = 0;
  setMultipleMotors(0, 0);
  int abs_yaw = getYaw();
  if (abs_yaw < 0) {
    abs_yaw *= -1;
    neg = true;
  }
  abs_yaw += ((abs_yaw % 90 >= 45 /* should round up?*/) * (90 - (abs_yaw % 90)) /*round up*/) + ((abs_yaw % 90 < 45 /*should round down?*/) * (-1 * (abs_yaw % 90)) /*round down*/);  //rounds yaw to nearest 90
  if (abs_yaw >= 360) {
    abs_yaw -= 360;
  }
  if (neg) {
    abs_yaw *= -1;
  }
  target = abs_yaw + deg;


  if (target < 0) {
    target += 360;
  }


  else if (target > 360) {
    target -= 360;
  }
  Serial.print("Degree: ");
  Serial.println(deg);


  Serial.print("target: ");
  Serial.println(target);
  int calc_speed = ((deg * -1) * speed) / abs(deg);
  Serial.print("\n\n\n\n\n\n\n\n\n\n\nRIGHT MOTOR: ");
  Serial.println(calc_speed);
  Serial.print("LEFT MOTOR: ");
  Serial.println(-calc_speed);


  while ((yaw > target + 6) || (yaw < target - 6)) {
    //Serial.println(yaw);
    setMultipleMotors(-calc_speed, calc_speed);
    yaw = getYaw();
  }


  setMultipleMotors(0, 0);
  delay(300);


  if (deg < 0) {
    setMultipleMotors(90, -90);
    delay(50);
  } else {
    setMultipleMotors(-90, 90);
    delay(50);
  }


  setMultipleMotors(0, 0);
  Serial.println(getYaw());
}




void bnoSetup() {
  int x;
  if (!(x = bno.begin(OPERATION_MODE_IMUPLUS))) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    Serial.println(x);
    while (1) {
      Serial.println("hi");
      delay(500);
    }
  }
}


//GREENNNNNNNNN


void setMultipleMotors(int left, int right) {  //set motors
                                              //int buff = right;
                                               //right = left;
                                               //left = buff;
  int boost = 0;                               //in case motors are slow
  //port 1
  left = left > 255 ? 255 : left;
  left = left < -255 ? -255 : left;
  if (left > 0) {


    digitalWrite(35, LOW);
    delayMicroseconds(5);
    digitalWrite(34, HIGH);
    analogWrite(12, left);  //pwm pin 12
  } else if (left < 0) {
#ifdef main_bot
    left -= boost;
#endif
    digitalWrite(34, LOW);
    delayMicroseconds(5);
    digitalWrite(35, HIGH);
    analogWrite(12, -left);
  } else {
    digitalWrite(34, LOW);
    digitalWrite(35, LOW);
    analogWrite(12, 0);
  }


  right = left > 255 ? 255 : right;
  right = right < -255 ? -255 : right;
  right = -right;
  if (right > 0) {
#ifdef main_bot
    right += boost;
#endif
    digitalWrite(36, LOW);
    delayMicroseconds(5);
    digitalWrite(37, HIGH);
    analogWrite(8, right);
  } else if (right < 0) {
#ifdef main_bot
    right -= boost;
#endif
    digitalWrite(37, LOW);
    delayMicroseconds(5);
    digitalWrite(36, HIGH);
    analogWrite(8, -right);
  } else {
    digitalWrite(36, LOW);
    digitalWrite(37, LOW);
    analogWrite(8, 0);
  }
}


float cm_to_encoders(float cm) {
  const float wheelDiameter = 6.42;
  return cm / (wheelDiameter * PI) * 360;
}


void Interruptfunc() {  //interrupt func for encoders
  if (digitalRead(31) == HIGH) {
    enc++;
  } else {
    enc--;
  }
}


void forward_enc(int encoders, int motor_speed) {  //go forward for enc
  int previousEnc = enc;
  while (enc <= previousEnc + encoders) {
    setMultipleMotors(motor_speed, motor_speed);
    Serial.println(enc);
  }
  setMultipleMotors(0, 0);
}
void backward_enc(int encoders, int motor_speed) {  //go backward for enc
  int previousEnc = enc;


  while (enc >= previousEnc - encoders) {
    setMultipleMotors(-motor_speed, -motor_speed);
    Serial.println(enc);
  }
  setMultipleMotors(0, 0);
}
void backwardCm(float dist, int motor_speed) {  //go backward for cm
  backward_enc(cm_to_encoders(dist), motor_speed);
}
void forwardCm(float dist, int motor_speed) {  //go forward for cm
  forward_enc(cm_to_encoders(dist), motor_speed);
}

void seesaw() {
  arm.write(90);
  while (getRoll() > 10) {
    setMultipleMotors(70, 170);
  }
  setMultipleMotors(0, 0);
  delay(1000);
  while (getRoll() < -5) {
    setMultipleMotors(5, 5);
  }
  arm.write(45);
}
void seesawUp() {
  arm.write(90);
  while(getRoll() > 15){
    setMultipleMotors(70, 70);
  }
}
void seesawDown() {
  arm.write(90);
  while (getRoll() < -15) {
    setMultipleMotors(10, 10);
  }
}

int getnum(char *p) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    sum *= 10;
    sum += (*(p + i) - '0');
  }
  return sum;
}
void collectVictim() {
  forwardCm(3, 60);
  enc_turn(120, 60);
  while (getBackDistance() > 20.0) {
    setMultipleMotors(55, -55);
  }
  while(getBackDistance() > 13.0){
    setMultipleMotors(-30, -30);
  }
  setMultipleMotors(0, 0);
  arm.write(100);
  delay(1000);
  arm.write(45);
  delay(1000);
  arm.write(100);
  delay(1000);
  arm.write(45);
  delay(1000);
  arm.write(100);
  delay(1000);
  arm.write(45);
  delay(1000);
  arm.write(100);
  delay(1000);
  arm.write(45);
  delay(1000);
}
void deliverVictim() {
  cam.write(0);
  setMultipleMotors(0, 0);
  while (getFrontDistance() > 5.0) {
    setMultipleMotors(70, 70);
  }
  setMultipleMotors(0, 0);
  arm.write(80);
  delay(1000);
  arm.write(0);
  delay(1000);
  arm.write(80);
  delay(1000);
  arm.write(0);
  delay(1000);
  backwardCm(5, 80);
  cam.write(85);
  write("i");
  read();
}
void func(){
  int least = -45;
  float least_val = 1000.0;
  enc_turn(-46, 80);
  setMultipleMotors(0, 0);
  for(int i = 0; i < 91; i+=5){
      enc_turn(3, 90);
      setMultipleMotors(0, 0);
      delay(250);
      float dist = getDistance();
      if(dist < least_val){
        least = i-45;
        least_val = dist;
      } 
  }
  while(getBackDistance() > least_val+1){
    setMultipleMotors(-70, 70);
  }
  setMultipleMotors(0,0);
}
