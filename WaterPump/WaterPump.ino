// https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/
#include<WiFi.h>
#include<ESPAsyncWebServer.h>
#include"page.h"
#include"variable.h"
        
AsyncWebServer server(80);

String formatTime(void)
{
    return String(hours) + ":" + String(minutes) + ":" + String(seconds);
}

void setup(){
    Serial.begin(115200);
    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
    pinMode(33, OUTPUT);
    digitalWrite(33, LOW);
    
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
        }
        request->send(200, "text/plain", "OK");
    });

    server.on("/currTime", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/plain", formatTime().c_str());
    });

    server.on("/toggleButtonGet", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String response = "";
        if(digitalRead(RELAY_PIN))
            response = "<a onclick=\"togglePumping()\"><button class=\"buttonRunNow\" style=\"background: red \">STOP</button></a>";
        else
            response = "<a onclick=\"togglePumping()\"><button class=\"buttonRunNow\" style=\"background: green \">START</button></a>";
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
            runPumpTime = request->getParam("t")->value().toInt();
        request->send(200, "text/plain", "OK");
    });

    server.on("/runPumpIntervalGet", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/plain", String(runPumpTime).c_str());
    });

    server.on("/regularPumping", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("r"))
            regularPumping = request->getParam("r")->value().toInt();;
        request->send(200, "text/plain", "OK");
    });

    server.on("/regularPumpingGet", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        String response = "";
        if(regularPumping)
            response="true";
        else
            response="false";
        request->send_P(200, "text/plain", response.c_str());
    });

    server.on("/runPumpPeriod", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("p"))
            runPeriod_INT = request->getParam("p")->value().toInt();
        request->send(200, "text/plain", "OK");
    });
    
    server.on("/periodGet", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        request->send_P(200, "text/plain", String(runPeriod_INT).c_str());
    });
    
    server.begin();
}

void loop()
{
    delay(1000);
    seconds++;
    
    milliSec = millis();

    /*switch runPeriod_INT
    {
        case 1: runPeriod = 12;
        break;
        case 2: runPeriod = 12;
        break;
        case 3: runPeriod = 12;
        break;
        default: runPeriod = 12;
        break;
    }*/
    
    if(pumpRUN && milliSecPump < runPumpTime*60000)
    {
        milliSecPump = milliSec - milliSecPumpStart;
    }
    else
    {
        pumpRUN = false;
        milliSecPump = 0;
        milliSecPumpStart = milliSec;
    }

    if(pumpRUN_AUTO && milliSecPump_AUTO < runPumpTime_AUTO*60000)
    {
        milliSecPump_AUTO = milliSec - milliSecPumpStart_AUTO;
    }
    else
    {
        pumpRUN_AUTO = false;
        milliSecPump_AUTO = 0;
        milliSecPumpStart_AUTO = milliSec;
    }
        


    
    
    if(pumpRUN || pumpRUN_AUTO)
        digitalWrite(RELAY_PIN, HIGH);
    else
        digitalWrite(RELAY_PIN, LOW);

}
