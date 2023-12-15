#define MSG_LENGTH 50
char msg[MSG_LENGTH];

void clearMessage(){
  for (int i = 0; i < strlen(msg); i++){
    msg[i] = '\0'
  }
}
void read() {
  //reads a message from Pi and stores it into msg[] 
  int i = 0;
  while (!(Serial.available()){}//wait for stuff to read

  clearMessage();
  while(Serial.available()){//reading
    char c = Serial.read();
    msg[i] = c;
    i++;
    delay(5); 
  }

}
void write(char message[]){
  for (int = 0; i < strlen(message); i++){
    Serial.write(message[i]);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);     //used for communication with the pi
  Serial2.begin(115200);  //used on arduino only
}

void loop() {
  // put your main code here, to run repeatedly:
  read();
  write(msg)
}
