#ifndef HEADER_H
#define HEADER_H
#pragma once
//----------------------------------------------------------------------------------------

//Libraries---------------------------:

#include <string.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>

//--------------------------

//Global defs--------------------------:

#define MSG_LENGTH 50

//---------------------

//Variables from all files---------------------------------:

//Comms

extern char msg[MSG_LENGTH];


//Evac

//Funcs

extern volatile int enc;

//Intersections

//LineTracing

//Obstacle
extern int trig;
extern int echo;


//-------------------------

//Functions from all files-----------------------------:

//Comms
void read();
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
void bnoSetup();
void enc_turn_abs(int deg, int speed);
void motorSetup();
//Intersections

//LineTracing

//Obstacle
void distanceISR();
float getFrontDistance();
bool seeObs (long dist);
void forward_until_obs(int dist);
//---------------------------------

//----------------------------------------------------------------------------------------
#endif
