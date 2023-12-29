#include "Header.h"

//Code for communicating with Raspberry
char msg[MSG_LENGTH];

void read(){
  int i = 0;
  memset(msg, 0, MSG_LENGTH);
  while (Serial2.available()) {
    char c = Serial2.read();
    msg[i] = c;
    i++;
    delay(10);
  }
}
void write(char message[]){
  for(int i = 0; i < strlen(message); i++){
    Serial2.write(message[i]);
    delay(5);
  }
}
