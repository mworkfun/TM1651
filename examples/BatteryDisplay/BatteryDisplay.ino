/***********************************************************
  Author:jieliang mo
  Date:15 June, 2020
  github: https://github.com/mworkfun/TM1651.git
  IDE:	  Arduino-1.8.9
  Version: 1.0.0
***************************************************************/

#include "TM1651.h"
#define CLK 3    //pins definitions for TM1651 and can be changed to other ports       
#define DIO 4
TM1651 batteryDisplay(CLK,DIO);

void setup(){
  batteryDisplay.init();
  batteryDisplay.set(2);//set brightness: 0---7
}

void loop(){
  batteryDisplay.displayLevel(0);//All off
  delay(400);
  batteryDisplay.displayLevel(7);//All on
  delay(400);
  charging();
}

void charging()
{
  for(uint8_t level = 0; level < 8; level ++)
  {
    batteryDisplay.displayLevel(level);
	delay(500);
  }
}
