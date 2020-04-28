#include <Wire.h>
#include "Balance.h"
 
LSM6 imu;
Balboa32U4Motors motors;
 
#Driver setup
void setup()
{
 
  ledYellow(0);
  ledRed(1);
  balanceSetup();
  ledRed(0);
}
 
#Driver run
void standUp()
{
  motors.setSpeeds(-SPEED_LIMIT, -SPEED_LIMIT);
  delay(400);
  motors.setSpeeds(150, 150);
  for (uint8_t i = 0; i < 20; i++)
  {
    delay(UPDATE_TIME_MS);
    if(angle < 60000)
    {
      break;
    }
  }
  motorSpeed = 150;
}
 
void loop()
{
 
  balanceUpdate();
 
}
