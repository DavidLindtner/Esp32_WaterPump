// https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/
#include<WiFi.h>
#include <ESP32Time.h>
#include<ESPAsyncWebServer.h>
#include <Wire.h>
#include"variable.h"
#include"page.h"

ESP32Time rtc;


AsyncWebServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(2, LOW);
    rtc.setTime(0, 0, 0, 1, 1, 2022);

    if (!EEPROM.begin(1000))
    {
        Serial.println("Failed to initialise EEPROM");
        Serial.println("Restarting...");
        delay(1000);
        ESP.restart();
    }
    loadData();
    
    WiFi.softAP(ssid, password);
    //Serial.println();
    //Serial.print("IP address: ");
    //Serial.println(WiFi.softAPIP());
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/html", index_html);
    });
    
    server.on("/setTime", HTTP_GET, [] (AsyncWebServerRequest *request)
    {
        if (request->hasParam("h") && request->hasParam("m") && request->hasParam("s"))
        {
            hours = request->getParam("h")->value().toInt();
            minutes = request->getParam("m")->value().toInt();
            seconds = request->getParam("s")->value().toInt();
            rtc.setTime(seconds, minutes, hours, 1, 1, 2021);
        }
        request->send(200, "text/plain", "OK");
    });

    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String response = "";
        response += String(period_INT) + "/";
        response += String(hours) + ":" + String(minutes) + ":" + String(seconds) + "/";
        if(pumpRUN)
            response += "1/";
        else
            response += "0/";
        if(regularPumping)
            response += "1/";
        else
            response += "0/";
        response += String(pumpTime) + "/";
        response += String(pumpTime_AUTO) + "/";
        response += String(pumpHour) + "/";
        response += String(pumpMinute) + "/";
        response += String(counterAct) + "/";
        request->send_P(200, "text/plain", response.c_str());
    });
    
    server.on("/toggleButton", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if(digitalRead(RELAY_PIN))
            pumpRUN = false;
        else
            pumpRUN = true;
        request->send(200, "text/plain", "OK");
    });
  
    server.on("/runPumpInterval", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("t"))
            pumpTime = request->getParam("t")->value().toInt();
        request->send(200, "text/plain", "OK");
    });

    server.on("/runPumpIntervalReg", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("t"))
            pumpTime_AUTO = request->getParam("t")->value().toInt();
        request->send(200, "text/plain", "OK");
    });

    server.on("/regularPumping", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("r"))
            regularPumping = request->getParam("r")->value().toInt();;
        request->send(200, "text/plain", "OK");
    });

    server.on("/runPumpPeriod", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("p"))
            period_INT = request->getParam("p")->value().toInt();
        request->send(200, "text/plain", "OK");
    });

    server.on("/pumpHour", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("h"))
            pumpHour = request->getParam("h")->value().toInt();
        request->send(200, "text/plain", "OK");
    });

    server.on("/pumpMinute", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("m"))
            pumpMinute = request->getParam("m")->value().toInt();
        request->send(200, "text/plain", "OK");
    });

    server.begin();
}

void loop()
{
 
    //------------------------------------------------------------------------------------------------------------------------
    //  Get time
    //------------------------------------------------------------------------------------------------------------------------
    milliSec = millis();
    hours = rtc.getHour(true);
    minutes = rtc.getMinute();
    seconds = rtc.getSecond();

    //------------------------------------------------------------------------------------------------------------------------
    //  Regular pumping - value change
    //------------------------------------------------------------------------------------------------------------------------
    if(regularPumping != regularPumping_OLD || pumpTime_AUTO != pumpTime_AUTO_OLD || period_INT != period_INT_OLD || pumpHour != pumpHour_OLD || pumpMinute != pumpMinute_OLD)
    {
        valueChange = true;
        storeData();
    }

    regularPumping_OLD = regularPumping;
    pumpTime_AUTO_OLD = pumpTime_AUTO;
    period_INT_OLD = period_INT;
    pumpHour_OLD = pumpHour;
    pumpMinute_OLD = pumpMinute;
    
    //------------------------------------------------------------------------------------------------------------------------
    //  Start regular pumping
    //------------------------------------------------------------------------------------------------------------------------

    //__________________________________________
    //  Get period in minutes
    //__________________________________________
    switch(period_INT)
    {
      case 1:
          periodMinute = 720;
          break;
      case 2:
          periodMinute = 1440;
          break;
      case 3:
          periodMinute = 2880;
          break;
      case 4:
          periodMinute = 4320;
          break;
      case 5:
          periodMinute = 5760;
          break;
      case 6:
          periodMinute = 7200;
          break;
      case 7:
          periodMinute = 8640;
          break;
      case 8:
          periodMinute = 10080;
          break;
    }

    if((hours == pumpHour && minutes == pumpMinute && valueChange == true) || (counterMin == periodMinute && valueChange == false))
    {
        pumpRUN_AUTO = true;
        valueChange = false;
        counterMin = 0;
    }

    //------------------------------------------------------------------------------------------------------------------------
    //  Timers
    //------------------------------------------------------------------------------------------------------------------------

    //__________________________________________
    //  Turn off one time pumping 
    //__________________________________________
    if(pumpRUN && milliSecPump < pumpTime*60000)
        milliSecPump = milliSec - milliSecPumpStart;
    else
    {
        pumpRUN = false;
        milliSecPump = 0;
        milliSecPumpStart = milliSec;
    }
    
    //__________________________________________
    //  Turn off regullar pumping 
    //__________________________________________
    if(pumpRUN_AUTO && milliSecPump_AUTO < pumpTime_AUTO*60000)
        milliSecPump_AUTO = milliSec - milliSecPumpStart_AUTO;
    else
    {
        pumpRUN_AUTO = false;
        milliSecPump_AUTO = 0;
        milliSecPumpStart_AUTO = milliSec;
    }

    //__________________________________________
    //  Connection active timer
    //__________________________________________
    if(milliSecCounterAct < 1000)
    {
        milliSecCounterAct = milliSec - milliSecCountActStart;
    }
    else
    {
        if(counterAct > 1000)
          counterAct = 0;
        else
          counterAct++;
          
        milliSecCounterAct = 0;
        milliSecCountActStart = milliSec;
    }
    
    //__________________________________________
    //  Minute counter
    //__________________________________________
    if(milliSecCounterMin < 60000)
    {
        milliSecCounterMin = milliSec - milliSecCountMinStart;
    }
    else
    {
        counterMin++;
        //storeData();
        milliSecCounterMin = 0;
        milliSecCountMinStart = milliSec;
    }

   
    //------------------------------------------------------------------------------------------------------------------------
    //  Writing to outputs
    //------------------------------------------------------------------------------------------------------------------------
    if(pumpRUN || (regularPumping && pumpRUN_AUTO))
        digitalWrite(RELAY_PIN, HIGH);
    else
        digitalWrite(RELAY_PIN, LOW);

    delay(100);
    
}
