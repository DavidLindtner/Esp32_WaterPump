#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#define RELAY_PIN 2

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

long milliSec = 0;

long milliSecPump = 0;
long milliSecPumpStart = 0;

long milliSecPump_AUTO = 0;
long milliSecPumpStart_AUTO = 0;

int hours = 0;
int minutes = 0;
int seconds = 0;

int runPumpTime = 3;
int runPumpTime_AUTO = 3;
int runPeriod_INT = 2;
int runPeriod = 2;

bool regularPumping = true;
bool pumpRUN = false;
bool pumpRUN_AUTO = false;


#endif /*__VARIABLE_H__*/
