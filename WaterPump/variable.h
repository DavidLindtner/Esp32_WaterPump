#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "EEPROM.h"

#define RELAY_PIN 2

const char *ssid = "Cerpadlo";
const char *password = "cerpadlo1";

//------------------------------------------------------------------------------------------------------------------------
//  Counters and timers variables
//------------------------------------------------------------------------------------------------------------------------
long milliSec = 0;

long milliSecPump = 0;
long milliSecPumpStart = 0;

long milliSecPump_AUTO = 0;
long milliSecPumpStart_AUTO = 0;

long milliSecCounterAct = 0;
long milliSecCountActStart = 0;

long milliSecCounterMin = 0;
long milliSecCountMinStart = 0;

int counterAct = 0;
int counterMin = 0;

//------------------------------------------------------------------------------------------------------------------------
//  Time variables
//------------------------------------------------------------------------------------------------------------------------
int hours = 0;
int minutes = 0;
int seconds = 0;

//------------------------------------------------------------------------------------------------------------------------
//  Manual pumping variables
//------------------------------------------------------------------------------------------------------------------------
bool pumpRUN = false;
int pumpTime = 3;

//------------------------------------------------------------------------------------------------------------------------
//  Automatic pumping variables
//------------------------------------------------------------------------------------------------------------------------
bool regularPumping = true;

int pumpTime_AUTO = 3;
int period_INT = 2;
int periodMinute = 0;

int pumpHour = 18;
int pumpMinute = 0;

bool regularPumping_OLD = true;
int pumpTime_AUTO_OLD = 3;
int period_INT_OLD = 2;
int pumpHour_OLD = 0;
int pumpMinute_OLD = 0;

bool valueChange = true;

bool pumpRUN_AUTO = false;


void storeData(void)
{
    EEPROM.writeInt(0, pumpTime);
    EEPROM.writeBool(4, regularPumping);
    EEPROM.writeInt(8, period_INT);
    EEPROM.writeInt(12, pumpTime_AUTO);
    EEPROM.writeInt(16, pumpHour);
    EEPROM.writeInt(20, pumpMinute);
    EEPROM.writeInt(24, hours);
    EEPROM.writeInt(28, minutes);
    EEPROM.writeInt(32, seconds);
    EEPROM.commit();
}

void loadData(void)
{
    pumpTime = EEPROM.readInt(0);
    regularPumping = EEPROM.readBool(4);
    period_INT = EEPROM.readInt(8);
    pumpTime_AUTO = EEPROM.readInt(12);
    pumpHour = EEPROM.readInt(16);
    pumpMinute = EEPROM.readInt(20);
    hours = EEPROM.readInt(24);
    minutes = EEPROM.readInt(28);
    seconds = EEPROM.readInt(32);
}


#endif /*__VARIABLE_H__*/
