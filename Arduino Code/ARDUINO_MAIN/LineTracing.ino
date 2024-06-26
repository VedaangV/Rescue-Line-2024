#include "Header.h"
#define CALIBRATION_ITERATIONS 100

const int sensorCount = 8;  //# of qtr array sensors
int bw_vals[sensorCount];   //array for qtr vals
float integral = 0.0;
float derivative = 0.0;
float last_error = 0.0;
int count = 0;
int leftSensor = 0;
int rightSensor = 7;
/*const float kp = 0.038;     //error multiplier
const float ki = 0.00001;  //integral multiplier
const float kd = 0.01;      //kd multiplier*/
const float kp = 0.019;
const float ki = 0.0;
const float kd = 0.0;
float error = 0.0;



QTRSensors qtr;


void setup_qtr() {
  //setup for the qtr sensor
  const uint8_t qtr_pins[] = {23, 25, 27, 30, 28, 26, 24, 22}; // main bot
  //const uint8_t qtr_pins[] = {22, 24, 26, 28, 30, 27, 25, 23}; //if order is revered on the backup bot
  qtr.setTypeRC();
  //qtr.setEmitterPin(2);
  qtr.resetCalibration();

  for (int i = 0; i < CALIBRATION_ITERATIONS; i++) {
    qtr.calibrate();
    qtr.setSensorPins(qtr_pins, sensorCount);
  }
}
float qtr_average(int start, int finish) {  //average of all sensors between start and finish. Endpoints included.
  int sum = 0;
  qtr.read(bw_vals);
  for (int i = start; i < finish + 1; i++) {
    sum += bw_vals[i];
  }
  return (sum / (finish - start + 1));
}

float error_calc() {

  //calculate the error for PID line tracing

  //desired difference between sensor pairs (ideally 0, but sensors are not perfect):

  float target_vals[] = {164, 0, -112, 0};  //5/20-storming. 8/*FINAL ROBOT*/
  float multipliers[] = { 2.3, 2.0, 1.5, 1.0};

  float error = 0.0;  //error in PID
  qtr.read(bw_vals);

  for (int i = 0; i < sensorCount / 2; i++) {            //iterates through all sensor pairs, outer to inner
    float current_val = (bw_vals[7 - i] - bw_vals[i]);   //difference between current pair of sensors
    float current_error = target_vals[i] - current_val;  //difference between desired val and actual val
    error += (current_error)*multipliers[i];             //sum up errors of each sensor pair
  }

  return error;
}
void qtr_print() {
  //print the qtr values in sensor order

  Serial.print("\n");
  for (int i = 0; i < sensorCount; i++) {
    qtr.read(bw_vals);
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(bw_vals[i]);
  }
}
void pid_print() {

  //print the error in PID
  Serial.print("PID error: ");
  Serial.println(error_calc());
}
void diff_print() {  //print the diff between sensor pairs.

  for (int i = 0; i < sensorCount / 2; i++) {
    qtr.read(bw_vals);
    Serial.print("Pair ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(bw_vals[7 - i] - bw_vals[i]);
  }
}

/*This handles all black intersection cases*/

void lineTrace(bool tCase) {  //main line tracking function
  int base_speed = 30;
  // gap();
  error = error_calc();                                              //calculating error
  integral += error;                                                       //summing up all erors during runtime
  derivative = error - last_error;                                         //checking the change in the errors over time.
  float adjustSpeed = (error * kp) + (integral * ki) + (derivative * kd);  //final number that calculates how much to adjust the motors.

  float leftSpeed = base_speed - adjustSpeed;
  float rightSpeed = base_speed + adjustSpeed;


  if (tCase && (leftBlack() || rightBlack()) && msg[1] == '1') { //if tCase enabled, see black on either left or right, and line ahead
        forwardCm(3, 50);
  }
  else{
    setMultipleMotors(leftSpeed, rightSpeed);
  }
  last_error = error;
}