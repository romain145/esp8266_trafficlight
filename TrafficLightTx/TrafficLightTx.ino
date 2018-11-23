
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

//const char* ssid = "ssid";
//const char* password = "password";
#include "wifi.h"

String serverIP = "192.168.1.200";

int ledPin = LED_BUILTIN; // GPIO2 of ESP8266;

int redPin = D4; // also LED_BUILTIN on ESP8266
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
//pinMode(ledPin, OUTPUT);
//digitalWrite(ledPin, HIGH); //OFF

pinMode(redPin, INPUT_PULLUP);
pinMode(yellowPin, INPUT_PULLUP);
pinMode(greenPin, INPUT_PULLUP);

Serial.println();
Serial.println();
Serial.print("Connecting to: ");
Serial.println(ssid);

WiFi.begin(ssid, password);
delay(1000);

/*
 The following four line of the 
 code will assign a Static IP Address to 
 the ESP Module. If you do not want this, 
 comment out the following four lines.  
 */

IPAddress ip(192,168,1,199);   
IPAddress gateway(192,168,1,1);   
IPAddress subnet(255,255,255,0);   
WiFi.config(ip, gateway, subnet);
//delay(5000);

while (WiFi.status() != WL_CONNECTED) 
{
  delay(200);
  Serial.print("*");
}
Serial.println(" connected");

// Print the IP address
Serial.print("IP: ");
Serial.println(WiFi.localIP());
}

void loop() 
{
  // RED
  if(red_prev == 0 && !digitalRead(redPin))
  {
    red_prev = 1;
    Serial.println("REDON");
    http.begin("http://" + serverIP + "/REDON");
    http.GET();
    http.end();
  }
  if(red_prev == 1 && digitalRead(redPin))
  {
    red_prev = 0;
    Serial.println("REDOFF");
    http.begin("http://" + serverIP + "/REDOFF");
    http.GET();
    http.end();
  }
  
  // YELLOW
  if(yellow_prev == 0 && !digitalRead(yellowPin))
  {
    yellow_prev = 1;
    Serial.println("YELLOWON");
    http.begin("http://" + serverIP + "/YELLOWON");
    http.GET();
    http.end();
  }
  if(yellow_prev == 1 && digitalRead(yellowPin))
  {
    yellow_prev = 0;
    Serial.println("YELLOWOFF");
    http.begin("http://" + serverIP + "/YELLOWOFF");
    http.GET();
    http.end();
  }
  
  // GREEN
  if(green_prev == 0 && !digitalRead(greenPin))
  {
    green_prev = 1;
    Serial.println("GREENON");
    http.begin("http://" + serverIP + "/GREENON");
    http.GET();
    http.end();
  }
  if(green_prev == 1 && digitalRead(greenPin))
  {
    green_prev = 0;
    Serial.println("GREENOFF");
    http.begin("http://" + serverIP + "/GREENOFF");
    http.GET();
    http.end();
  }

  delay(100);
  

}



