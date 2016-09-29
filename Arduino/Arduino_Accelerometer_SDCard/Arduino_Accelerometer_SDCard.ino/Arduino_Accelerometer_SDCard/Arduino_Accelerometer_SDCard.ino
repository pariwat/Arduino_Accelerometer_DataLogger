
#include "Wire.h"
#include "ADXL345.h"
#include <SPI.h>
#include <SD.h>
/************************************************************************
 * Arduino driver test for ADXL345 3-axes accelerometer                 *
 * Version 2.1                                                          *
 *                                                                      *
 * This program is free software; you can redistribute it and/or modify *
 * it under the terms of the GNU License V2.                            *
 * This program is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License, version 2 for more details               *
 *                                                                      *
 * Original development: Kevin Stevenard                                *
 * Modified by Justin Shaw May 2010                                     *
 * Modified by Jens Chr Brynildsen April 2012                           *
 ***********************************************************************/
 
// Cabling for i2c using Sparkfun's breakout with an Arduino Uno / Duemilanove:
// Arduino <-> Breakout board
// Gnd      -  GND
// 3.3v     -  VCC
// 3.3v     -  CS
// Analog 4 -  SDA
// Analog 5 -  SLC

// Cabling for i2c using Sparkfun's breakout with an Arduino Mega / Mega ADK:
// Arduino <-> Breakout board
// Gnd      -  GND
// 3.3v     -  VCC
// 3.3v     -  CS
// 20       -  SDA
// 21       -  SLC


const int chipSelect = 20;

ADXL345 accel;
int yy=0;
double Energy=0,max_a=0;
double data[20];
double energy=0;
double buf=0;
File dataFile;

void setup(void){
  int x, y, z, i;
  double xyz[3], gains[3], gains_orig[3];

  Serial.begin(57600);
  Serial.println("Checking... ");
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  Serial.println("System OK. runing...");
  dataFile = SD.open("test.txt", FILE_WRITE);
  accel.powerOn();
  
  accel.getAxisGains(gains_orig);
  Serial.println("gains_orig[]:");
  for(i = 0; i < 3; i++){
    Serial.print(gains_orig[i], 6);
    Serial.print(" ");
  }
  Serial.println("");
  
  gains[0] = .1;
  gains[1] = 1.1;
  gains[2] = 2.1;
  accel.setAxisGains(gains);
  accel.getAxisGains(gains);
  accel.setAxisGains(gains_orig);
  accel.getAxisGains(gains);
  accel.readAccel(&x, &y, &z);
  accel.get_Gxyz(xyz);
  accel.setTapThreshold(1);
  accel.setAxisOffset(2, 3, 4);
  accel.getAxisOffset(&x, &y, &z);
  accel.setTapDuration(5);
  accel.setDoubleTapLatency(6);
  accel.setDoubleTapWindow(7);
  accel.setActivityThreshold(8);
  accel.setInactivityThreshold(9);
  accel.setTimeInactivity(10);
  accel.setFreeFallThreshold(11);
  accel.setFreeFallDuration(12);
  accel.setActivityAc(true);
  accel.setInactivityAc(true);
  accel.setSuppressBit(true);
  accel.setSuppressBit(false);
  accel.setTapDetectionOnX(true);
  accel.setTapDetectionOnX(false);
  accel.setTapDetectionOnY(true);
  accel.setTapDetectionOnY(false);
  accel.setTapDetectionOnZ(true);
  accel.setTapDetectionOnZ(false);
  accel.setActivityX(true);
  accel.setActivityY(true);
  accel.setActivityZ(true);

  accel.setInactivityX(false);
  accel.setInactivityY(false);
  accel.setInactivityZ(false);
  accel.setLowPower(false);
  accel.setRate(3.14159);  
  accel.setInterruptMapping(1, true);
  accel.setInterrupt(1, true);
  accel.setSelfTestBit(false);
  accel.printAllRegister();
  
}


void loop(void){
  int x, y, z;
  accel.readAccel(&x, &y, &z);

  double SVM = sqrt(pow(x,2) + pow(y,2) + pow(z,2));                                        // Pre-Processing Signal Vector Magnitude (SVM)
  
  if(yy <20)
  {
    data[yy] = SVM * 1.0;
  }else
  {
    max_a = 0;
    int i=0;
    for(i=4;i<20;i++)
    {
      Energy = ((0.125) * ( (2.0*data[i]) + data[i-1] - data[i-3] - (2.0*data[i-4]) ) );    // Energy 
      Energy = sqrt(pow(Energy,2));                                                         // 
      if(Energy > max_a)
        max_a = Energy;
    }    
    yy=-1;
  }
  yy++;
  
  Serial.println(Energy, DEC);

  delay(100);                                                                               // Sampling data At 100 ms or 10 Hz
}
