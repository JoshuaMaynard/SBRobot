#pragma once 
 
  
 
#include <stdint.h> 
 
#include <LSM6.h> 
 
#include <Balboa32U4.h> 
 
  
 
// Gear ratio to deal with the constant size of the wheels, 90mm 
 
const int16_t G_RATIO = 111; 
 
  
 
// Controlling the max speed of motor 
 
const int16_t SPEED_LIMIT = 300; 
 
  
 
// Angle of the robot in regard to the rate of change 
 
// of the robot when it is either falling or rising compared 
 
// to the desired position 
 
const int16_t ANGLE_RATIO = 140; 
 
  
 
// Uses angle and angle_rate to measure how far the robot 
 
// will move to go into a stable state. 
 
const int16_t ANGLE_CONTROL = 11; 
 
  
 
// Used to keep robot resisting being moved from where it begins 
 
const int16_t SPEED_CONTROL = 73; 
 
  
 
 
 
// Refreshing the time cycles sent to the robot to 
 
// send commands 
 
const uint8_t UPDATE_TIME_MS = 10; 
 
  
 
// Calibrating the gyro but taking 100 initial measurements. 
 
const uint8_t CALIBRATE = 100; 
 
  
 
// From vertical position, these are angles (in millidegrees) 
 
// that the robot will start and stop trying to balance.  
 
// The different values start balancing at 45 degrees and 
 
// stop at 70 degrees from vertical position. 
 
const int32_t START_ANGLE = 45000; 
 
const int32_t STOP_ANGLE = 70000; 
 
  
 
// Variables needed for balancing calculations 
 
extern int32_t angle;  
 
extern int32_t angleRate;  
 
extern int16_t motorSpeed;  
 
  
 
// Variables needed to communicate to robot 
 
extern LSM6 imu; 
 
extern Balboa32U4Motors motors; 
 
extern Balboa32U4Encoders encoders; 
 
  
 
// Call function to calibrate gyro 
 
void balanceSetup(); 
 
  
 
// Call to run balancing algorithm 
 
void balanceUpdate(); 
 
  
 
// T/F option to keep robot from moving when falling over  
 
// and return true when working correctly and continuing 
 
bool isBalancing();
