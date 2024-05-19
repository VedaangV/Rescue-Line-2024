#ifndef HEADER_H
#define HEADER_H
#pragma once
//----------------------------------------------------------------------------------------

//Libraries---------------------------:

#include <string.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <QTRSensors.h>//for qtr
#include <Servo.h>
//--------------------------

//Global defs--------------------------:

#define MSG_LENGTH 500

//---------------------

//Variables from all files---------------------------------:
#define BLACK_THRESH 1150
#define WHITE_THRESH 700

#define numOfBlack() ((bw_vals[0] > BLACK_THRESH) + (bw_vals[1] > BLACK_THRESH) + (bw_vals[2] > BLACK_THRESH) + (bw_vals[3] > BLACK_THRESH) + (bw_vals[4] > BLACK_THRESH) + (bw_vals[5] > BLACK_THRESH) + (bw_vals[6] > BLACK_THRESH) + (bw_vals[7] > BLACK_THRESH))
#define rightBlack() ((bw_vals[7] > BLACK_THRESH) && (bw_vals[6] > BLACK_THRESH) && (bw_vals[5] > BLACK_THRESH) && (bw_vals[4] > BLACK_THRESH) && (bw_vals[3] > BLACK_THRESH))
#define leftBlack() ((bw_vals[0] > BLACK_THRESH) && (bw_vals[1] > BLACK_THRESH) && (bw_vals[2] > BLACK_THRESH) && (bw_vals[3] > BLACK_THRESH) && (bw_vals[4] > BLACK_THRESH))

//Comms

extern char msg[MSG_LENGTH];


//Evac

//Funcs

extern volatile int enc;

//Intersections

//LineTracing
extern int bw_vals[];
extern int leftSensor;
extern int rightSensor;
extern QTRSensors qtr;
//Obstacle
extern int trig;
extern int echo;
extern float error;


//-------------------------

//Functions from all files-----------------------------:
//main
extern Servo arm;
extern Servo cam;
//Comms
void read();
void write(char message[]);
//Evac

//Funcs
void Interruptfunc();
void setMultipleMotors(int left, int right);
int getnum(char *p);
float cm_to_encoders(float cm);
void Interruptfunc();
void forward_enc(int encoders, int motor_speed);
void backward_enc(int encoders, int motor_speed);
void backwardCm(float dist, int motor_speed);
void forwardCm(float dist, int motor_speed);
void enc_turn(int deg, int speed);
float getYaw();
float getPitch();
float getRoll();
void bnoSetup();
void enc_turn_abs(int deg, int speed);
void motorSetup();
float getDistance();
float getBackDistance();
void seesaw();
void seesawUp();
void seesawDown();
void collectVictim();
void deliverVictim();
void func();
//Intersections

//LineTracing
void setup_qtr();
float qtr_average(int start, int finish);
float error_calc();
void qtr_print();
void pid_print();
void diff_print();
void lineTrace(bool tCase);
//Obstacle
void distanceISR();
float getFrontDistance();
bool seeObs (long dist);
void forward_until_obs(int dist);
float tofSetup();
//---------------------------------

//----------------------------------------------------------------------------------------
#endif
