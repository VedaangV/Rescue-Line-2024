#include <Adafruit_BNO055.h>
#include "Adafruit_APDS9960.h"
#include "Header.h"
#include <Servo.h>
Adafruit_APDS9960 color;
int x = 0;
Servo arm;
Servo cam;
bool armDown = 0;
bool evac = false;
void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:

  Serial2.begin(9600);
  Serial.begin(115200);
  Wire.begin();
  tofSetup();
  attachInterrupt(digitalPinToInterrupt(18), Interruptfunc, RISING);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  setup_qtr();
  //write("init");
  bnoSetup();
  if (!color.begin()) {
    Serial.println("color failed");
    for (;;)
      ;
  }
  color.enableColor(true);
  arm.attach(A6);
  cam.attach(A9);
  Serial.println("Cleared setup");
  arm.write(45);
  delay(100);
  cam.write(120);
}


void loop() {
  // put your main code here, to run repeatedly:
  if (getRoll() > 15) {
    seesawUp();
  }
  if (getRoll() < -15) {
    seesawDown();
  }
  arm.write(45);

  read();
  uint16_t r, g, b, c;
  color.getColorData(&r, &g, &b, &c);
  if (c > 650) {  //sees silver
    setMultipleMotors(0, 0);
    while (1) {}
  }
  if (x % 10 == 0) {
    obstacle();
  }
  Serial.println(msg);
  //Serial.println("hi");
  if (msg[0] == '+' || msg[0] == '-') {
    int sign1 = 1 - (2 * (msg[0] == '-')), sign2 = 1 - (2 * (msg[4] == '-'));
    setMultipleMotors(getnum(&msg[1]) * sign1, getnum(&msg[5]) * sign2);
  }
  if (msg[0] == 'F' && msg[1] == 'S') {    //going forward (or backward)
    int sign = 1 - (2 * (msg[2] == '-'));  //sets sign to 1 or -1
    setMultipleMotors(getnum(&msg[3]) * sign, getnum(&msg[3]) * sign);
    Serial.println(getnum(&msg[3]));
  }
  if (msg[0] == 'F' && msg[1] == 'C') {  //going forward with cm
    forwardCm(getnum(&msg[2]), 60);
  }
  if (msg[0] == 'B' && msg[1] == 'C') {  //going backward with cm
    backwardCm(getnum(&msg[2]), 60);
  }
  if (msg[0] == 'R' && msg[1] == 'G') {
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

  } else if (msg[0] == 'L' && msg[1] == 'G') {
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

  } else if (msg[0] == 'D' && msg[1] == 'G') {
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

  } else if (msg[0] == 'Z') {
    lineTrace(true);
  } else if (msg[0] == 'X' && msg[1] == 'Y' && msg[2] == 'Z') {
    enc_turn(-90, 75);
    setMultipleMotors(0, 0);
    delay(3000);
  } else if (msg[0] == 'a') {
    setMultipleMotors(50, -50);
    delay(40);
    setMultipleMotors(0, 0);
    delay(40);
  } else if (msg[0] == 'b') {
    setMultipleMotors(-50, 50);
    delay(40);
    setMultipleMotors(0, 0);
    delay(40);
  } else if (msg[0] == 'V') {
    while (getFrontDistance() > 8.0) {
      setMultipleMotors(50, 50);
    }
    collectVictim();
    write("u");
  } else if (msg[0] == 'p') {
    deliverVictim();
    write("i");
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
