
// RXI: GPIO1
// LED: GPIO2
// TXO: GPIO3

// CANNOT USE 5 or 6 for some reason... causes a wdt reset

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//const char* ssid = "ssid";
//const char* password = "password";
#include "wifi.h"

String serverIP = "192.168.0.1";

int ledPin = LED_BUILTIN; // GPIO2 of ESP8266;

int redPin = D4; // LED_BUILTIN or pin #2 on ESP8266
int yellowPin = D3;
int greenPin = D2;
int fourPin = D1;

int red_prev = 0;
int yellow_prev = 0;
int green_prev = 0;

int i = 0;

HTTPClient http;

void setup() 
{
Serial.begin(115200);
pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, HIGH); //OFF

//pinMode(redPin, INPUT);
//pinMode(yellowPin, INPUT);
//pinMode(greenPin, INPUT);

Serial.println();
Serial.println();
Serial.print("Connecting to: ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) 
{
  delay(100);
  Serial.print("*");
}
Serial.println("");
Serial.println("WiFi connected");

// Print the IP address
Serial.print("ESP8266 IP: ");
Serial.print(WiFi.localIP());
}

void loop() 
{
  // RED
  if(red_prev == 0 && digitalRead(redPin))
  {
    red_prev = 1;
    http.begin("http://" + serverIP + "/REDON");
    http.GET();
    http.end();
  }
  if(red_prev == 1 && !digitalRead(redPin))
  {
    red_prev = 0;
    http.begin("http://" + serverIP + "/REDOFF");
    http.GET();
    http.end();
  }
  
  // YELLOW
  if(yellow_prev == 0 && digitalRead(yellowPin))
  {
    yellow_prev = 1;
    http.begin("http://" + serverIP + "/YELLOWON");
    http.GET();
    http.end();
  }
  if(yellow_prev == 1 && !digitalRead(yellowPin))
  {
    yellow_prev = 0;
    http.begin("http://" + serverIP + "/YELLOWOFF");
    http.GET();
    http.end();
  }
  
  // GREEN
  if(green_prev == 0 && digitalRead(greenPin))
  {
    green_prev = 1;
    http.begin("http://" + serverIP + "/GREENON");
    http.GET();
    http.end();
  }
  if(green_prev == 1 && !digitalRead(greenPin))
  {
    green_prev = 0;
    http.begin("http://" + serverIP + "/GREENOFF");
    http.GET();
    http.end();
  }

  delay(100);
  

}



