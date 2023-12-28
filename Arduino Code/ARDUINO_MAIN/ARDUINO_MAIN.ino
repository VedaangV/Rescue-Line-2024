#include "Header.h"

//Main code

void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600);
  Serial.begin(115200);
  write("init");
}

void loop() {
  // put your main code here, to run repeatedly:

  read();
  Serial.println(msg);
}