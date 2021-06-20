// https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/
#include<WiFi.h>
#include<ESPAsyncWebServer.h>
#include"page.h"

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

int hours = 0;
int minutes = 0;
int seconds = 0;
        
AsyncWebServer server(80);


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
        request->send_P(200, "text/html", index_html, processor);
    });
    
    server.on("/time", HTTP_GET, [] (AsyncWebServerRequest *request) {
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

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    if (request->hasParam("output") && request->hasParam("state"))
    {
      String inputMessage1 = request->getParam("output")->value();
      String inputMessage2 = request->getParam("state")->value();
      Serial.println("Menime stav LED");
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    request->send(200, "text/plain", "OK");
  });
    
    server.begin();
}

void loop(){}
