#include <Wire.h>
#include "Balance.h"
 
int32_t angleRate; // degrees/s
int32_t angle; // millidegrees
int16_t turnSpeed;
int32_t gYZero;
bool balanceStatus;
bool BalanceBool = false;
 
//Starts the robot
void balanceSetup()
{
  //Start
  Wire.begin();
  imu.writeReg(LSM6::CTRL2_G, 0b01011000); // 208 Hz, 1000 deg/s
  imu.enableDefault();
  delay(500);
 
  // Calibrate the gyro.
  int32_t gyroTotal = 0;
  for (int i = 0; i < CALIBRATE; i++)
  {
    imu.read();
    gyroTotal += imu.g.y;
    delay(1);
  }
 
  gYZero = gyroTotal / CALIBRATE;
}
 
// Primary function for balancing the robot
// This function loops every 10 milliseconds 
void balance()
{
  // angle is a continuously updated variable 
  // Adjust toward angle = 0 degrees
  // At 0 degrees the angle is balanced
  // Relies on the average of variable "angle" being 0. 
  // Below ensures the angle is slightly adjusted every loop, forcing the robot to move to achieve 0 average
  angle = angle * 999 / 1000;
  
  // This is in milidegrees and determines how far off of 0 we are.
  // angleRate is the raw reading from the gyroscope
  // ANGLE_RATIO is the raw gear ratio from the wheels
  int32_t risingAngleOffset = angleRate * ANGLE_RATIO + angle;
 
  // Combine risingAngleOffset with the distance and speed variables to get our motor response.
  // The following is the primary algorithm for adjusting the robot. 
  // The idea behind this is to add the necessary speed amount to counteract the falling speed
  // ANGLE_CONTROL, SPEED_CONTROL, and GEAR_RATIO are hardcoded values
  turnSpeed += (
    + ANGLE_CONTROL * risingAngleOffset
    + SPEED_CONTROL
    ) / 100 / GEAR_RATIO;
 
  // Prevent the speeds from exceeding their hardcoded limits.
  if (turnSpeed > SPEED_LIMIT)
  {
    turnSpeed = SPEED_LIMIT;
  }
  if (turnSpeed < -SPEED_LIMIT)
  {
    turnSpeed = -SPEED_LIMIT;
  }
  
  //Set the motor speeds accordingly
  motors.setSpeeds(turnSpeed,turnSpeed);
}
 
//Returns true/false
bool getBalance()
{
  return BalanceBool;
}
 
 
//Converts the gyroscope's values into something usable
void integrateGyro()
{
  // Convert from full-scale 1000 deg/s to deg/s.
  angleRate = (imu.g.y - gYZero) / 29;
  angle += angleRate * UPDATE_TIME_MS;
}
 
//Updates the gyroscope
void balanceUpdateSensors()
{
  imu.read();
  integrateGyro();
}
 
void balanceUpdate()
{
  static uint16_t lastMillis;
  uint16_t ms = millis();
  static uint8_t count = 0;
 
  // Perform the balance updates at 100 Hz.
  if ((uint16_t)(ms - lastMillis) < UPDATE_TIME_MS) { return; }
  balanceStatus = ms - lastMillis > UPDATE_TIME_MS + 1;
  lastMillis = ms;
 
 
  if (BalanceBool == true)
  {
    balance();
 
    // Stop trying to balance if we have been farther from
    // vertical than STOP_ANGLE for 5 counts.
    if (abs(angle) > STOP_ANGLE)
    {
      if (++count > 5)
      {
        BalanceBool = false;
        count = 0;
      }
    }
    else
    {
      count = 0;
    }
  }
  else
  {
    // Start trying to balance if we have been closer to
    // vertical than START_ANGLE for 5 counts.
    if (abs(angle) < START_ANGLE)
    {
      if (++count > 5)
      {
        BalanceBool = true;
        count = 0;
      }
    }
    else
    {
      count = 0;
    }
  }
}
