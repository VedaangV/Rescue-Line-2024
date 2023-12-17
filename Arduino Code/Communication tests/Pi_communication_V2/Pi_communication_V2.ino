#define MSG_LENGTH 50
char msg[MSG_LENGTH];

void read(){
  int i = 0;
  while (Serial2.available()) {
    char c = Serial2.read();
    msg[i] = c;
    i++;
  }
}
void write(char message[]){
  for(int i = 0; i < strlen(message); i++){
    Serial2.write(message[i]);
    delay(5);
  }
}
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
