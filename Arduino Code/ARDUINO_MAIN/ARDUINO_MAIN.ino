#include <Adafruit_BNO055.h>
#include "Adafruit_APDS9960.h"
#include "Header.h"
#include <Servo.h>
Adafruit_APDS9960 color;
int x = 0;
Servo arm;
Servo cam;
bool armDown = 0;
int leftSpeed = 30;
int rightSpeed = 30;
int evac = 0;
void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
  Serial2.begin(9600);
  Serial.begin(115200);
  Serial.println("E");
  Wire.begin();
  tofSetup();
  Serial.println("TOF");
  attachInterrupt(digitalPinToInterrupt(18), Interruptfunc, RISING);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  setup_qtr();
  //write("init");
  bnoSetup();
  Serial.println("BNO");

  if (!color.begin()) {
    Serial.println("color failed");
    for (;;)
      ;
  }
  color.enableColor(true);
  
  arm.attach(A6);
  cam.attach(A9);
 
  Serial.println("Cleared setup!!");
  arm.write(45);
  delay(100);
  cam.write(130-(45*evac));
}


void loop() {
  // put your main code here, to run repeatedly:
  if(evac)
    cam.write(85);
  else
    cam.write(130);
    Serial.println("iugrbjh4ohbuip");
  
  if (getRoll() > 15 && !evac) {
    seesawUp();
  }
  if (getRoll() < -15 && !evac) {
    seesawDown();
  }
  arm.write(45);
  Serial.println(evac);
  read();
  uint16_t r, g, b, c;
  color.getColorData(&r, &g, &b, &c);
  Serial.print("clear: ");
  Serial.println(c);
  if (c > 650 && !evac) {  //sees silver
    Serial.println(evac);
    evac = 1;
    setMultipleMotors(0, 0);
    delay(1000);
    forwardCm(45, 70);
    enc_turn(-90, 45);
    if(getFrontDistance() < 15.0){
      enc_turn(180, 45);
    }
    forwardCm(60, 40);
    write("s");
  }
  if (x % 10 == 0 && !evac) {
    obstacle();
    setMultipleMotors(0, 0);
  }
  //Serial.println(msg);
  //Serial.println(msg[0]);
  //Serial.println("hi");
  if (msg[0] == '+' || msg[0] == '-') {
    Serial.println("SETTING MOTORS");
    int sign1 = 1 - (2 * (msg[0] == '-')), sign2 = 1 - (2 * (msg[4] == '-'));
    setMultipleMotors(getnum(&msg[1]) * sign1, getnum(&msg[5]) * sign2);
  }
  if (!evac && msg[0] == 'F' && msg[1] == 'S') {    //going forward (or backward)
    int sign = 1 - (2 * (msg[2] == '-'));  //sets sign to 1 or -1
    setMultipleMotors(getnum(&msg[3]) * sign, getnum(&msg[3]) * sign);
    Serial.println(getnum(&msg[3]));
  }
  if (!evac && msg[0] == 'R' && msg[1] == 'G') {
    while (!(leftBlack() || rightBlack())) {
      lineTrace(false);
    }
    setMultipleMotors(0, 0);
    delay(1000);
    forwardCm(6.5, 50);
    enc_turn(90, 70);
    //enc_turn_abs(90, 100);
    backwardCm(2.5, 60);
    setMultipleMotors(0, 0);
    delay(3000);
    read();

    setMultipleMotors(0, 0);
    delay(1000);

  } else if (!evac && msg[0] == 'L' && msg[1] == 'G') {
    while (!(leftBlack() || rightBlack())) {
      lineTrace(false);
    }
    setMultipleMotors(0, 0);
    delay(1000);
    forwardCm(4, 60);
    enc_turn(-90, 70);
    //enc_turn_abs(-90, 100);
    setMultipleMotors(0, 0);
    backwardCm(1, 60);
    delay(3000);
    read();

  } else if (!evac && msg[0] == 'D' && msg[1] == 'G') {
    while (!(leftBlack() || rightBlack())) {
      lineTrace(false);
    }
    setMultipleMotors(0, 0);
    delay(1000);
    //forwardCm(5, 60);
    enc_turn(180, 70);
    //enc_turn_abs(180, 100);
    setMultipleMotors(0, 0);
    backwardCm(4, 60);
    delay(3000);
    read();

  } else if (msg[0] == 'Z' && !evac) {
    Serial.println("IMRIGHTTTTTTTTTTTT!!!!!!");
    lineTrace(true);
    Serial.println("IMRIGHTTTTTTTTTTTT!!!!!!");
  }
    else if (msg[0] == 'a' && evac) {
    setMultipleMotors(60, -60);
    delay(50);
    setMultipleMotors(0, 0);
    delay(50);
  } else if (msg[0] == 'b' && evac) {
    setMultipleMotors(-60, 60);
    delay(50);
    setMultipleMotors(0, 0);
    delay(50);
  }/*else if (msg[0] == 'r' && evac){
    setMultipleMotors(60, -60);
    delay(50);
    setMultipleMotors(0, 0);
    delay(50);
    Serial.println("rrrrrrrrrrr");
  } */else if (msg[0] == 'V' && evac) {
    forwardCm(2, 70);
  } else if (msg[0] == 'p' && evac) {
    deliverVictim();
    enc_turn(90, 70);
    write("i");
  } else if (msg[0] == 'H' && evac) {
    collectVictim();
    write("u");
  }



  //enc_turn_abs(90, 75);
  //delay(2000);
  //while(true)
  //  setMultipleMotors(50, 50);
  //while(true)
  //setMultipleMotors(70, 70);
  //delay(3000);
  //enc_turn_abs(-90, 90);
  //
  x++;
}
