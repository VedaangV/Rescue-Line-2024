#include "Header.h"

void setup() {
  // put your setup code here, to run once:
  // put your setup code here, to run once:
  Serial2.begin(9600);
  Serial.begin(115200);
  Wire.begin();
  attachInterrupt(digitalPinToInterrupt(18), Interruptfunc, RISING);
  //write("init");
}

void loop() {
  // put your main code here, to run repeatedly:

  read();
  write("!");
  Serial.println(msg);
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
    forwardCm(getnum(&msg[2]), 120);
  }
  if (msg[0] == 'B' && msg[1] == 'C') {  //going backward with cm
    backwardCm(getnum(&msg[2]), 120);
  }
}

