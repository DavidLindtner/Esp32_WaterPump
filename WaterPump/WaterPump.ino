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
    
    Serial.println();
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });
    
    server.on("/setTime", HTTP_GET, [] (AsyncWebServerRequest *request) {
        if (request->hasParam("h") && request->hasParam("m") && request->hasParam("s"))
        {
            hours = request->getParam("h")->value().toInt();
            minutes = request->getParam("m")->value().toInt();
            seconds = request->getParam("s")->value().toInt();
            Serial.print(hours);
            Serial.print(":");
            Serial.print(minutes);
            Serial.print(":");
            Serial.println(seconds);
        }
        request->send(200, "text/plain", "OK");
    });

    server.on("/currTime", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println(formatTime());
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
        String response = "";
        if(digitalRead(RELAY_PIN))
            startPumping = false;
        else
            startPumping = true;
        request->send(200, "text/plain", "OK");
    });
  
    server.on("/runPumpInterval", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        Serial.println("sme tu");
        if (request->hasParam("t"))
        {
            runPumpTime = request->getParam("t")->value().toInt();
            Serial.println(runPumpTime);
        }
        request->send(200, "text/plain", "OK");
    });

    server.on("/runPumpIntervalGet", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("t"))
            runPumpTime = request->getParam("t")->value().toInt();
        request->send_P(200, "text/plain", String(runPumpTime).c_str());
    });
    
    server.begin();
}

void loop()
{
    delay(1000);
    seconds++;
    
    if(startPumping)
        digitalWrite(RELAY_PIN, HIGH);
    else
        digitalWrite(RELAY_PIN, LOW);

}
