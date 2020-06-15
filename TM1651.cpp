//  Author:jieliang mo
//  Date:15 June, 2020
//  github: https://github.com/mworkfun/TM1651.git
//  Applicable Module:
//                     Battery Display v1.0.0
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 1.0.0 of the License, or (at your option) any later version.
/*******************************************************************************/
#include "TM1651.h"
#include <Arduino.h>
static int8_t LevelTab[] = {0x00,0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f}; //Level 0~7

TM1651::TM1651(uint8_t Clk, uint8_t Data){
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin,OUTPUT);
  pinMode(Datapin,OUTPUT);
}

void TM1651::init(){
  set(BRIGHT_TYPICAL);
  clearDisplay();
}

void TM1651::writeByte(int8_t wr_data){
  uint8_t i,count1;   
  for(i=0;i<8;i++)        //sent 8bit data
  {
    digitalWrite(Clkpin,LOW);      
    if(wr_data & 0x01)
      digitalWrite(Datapin,HIGH);//LSB first
    else 
      digitalWrite(Datapin,LOW);
	  delayMicroseconds(3);
    wr_data >>= 1;      
    digitalWrite(Clkpin,HIGH);
	  delayMicroseconds(3);    
  }   
}
//send start signal to TM1651
void TM1651::start(void){
  digitalWrite(Clkpin,HIGH);//send start signal to TM1651
  digitalWrite(Datapin,HIGH);
  delayMicroseconds(2);
  digitalWrite(Datapin,LOW); 
  //delayMicroseconds(2);
  //digitalWrite(Clkpin,LOW); 
} 
//ack function
void TM1651::ack(void){
  int dy=0;
  digitalWrite(Clkpin,LOW); 
  delayMicroseconds(5);    
  pinMode(Datapin,INPUT);
  while(digitalRead(Datapin)){ //wait for the ACK
    delayMicroseconds(1);
    dy += 1;
    if(dy > 5000)  //Prevent infinite loop
      break;
  }
  digitalWrite(Clkpin,HIGH); 
  delayMicroseconds(2);    
  digitalWrite(Clkpin,LOW); 
  pinMode(Datapin,OUTPUT);   
}
//End of transmission
void TM1651::stop(void){
  digitalWrite(Clkpin,HIGH);
  digitalWrite(Datapin,LOW);
  delayMicroseconds(2);
  digitalWrite(Datapin,HIGH); 
}
//******************************************
void TM1651::displayLevel(uint8_t Level){
  if(Level > 7)return;   //Level should be 0~7
  start();               //start signal sent to TM1651 from MCU
  writeByte(ADDR_FIXED); //fixed address
  ack();
  stop();           
  start();          
  writeByte(STARTADDR);  //set the address
  ack();
  writeByte(LevelTab[Level]);//display data
  ack();
  stop();            
  start();          
  writeByte(Cmd_DispCtrl);   //Control the brightness 
  ack();
  stop();           
}
//clear display
void TM1651::clearDisplay(void){
  displayLevel(0);
}
//To take effect the next time it displays.
void TM1651::set(uint8_t brightness,uint8_t SetData,uint8_t SetAddr){
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
}
