
// RXI: GPIO1
// LED: GPIO2
// TXO: GPIO3

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

//const char* ssid = "ssid";
//const char* password = "password";
#include "wifi.h"

int redPin = D4;  // also LED_BUILTIN on ESP8266
int yellowPin = D3;
int greenPin = D2;
int fourPin = D1;

int i = 0;

ESP8266WebServer server(80);//Service Port

void setup() 
{
Serial.begin(115200);
pinMode(redPin, OUTPUT);
pinMode(yellowPin, OUTPUT);
pinMode(greenPin, OUTPUT);

Serial.println();
Serial.println();
Serial.print("Connecting to: ");
Serial.println(ssid);

WiFi.begin(ssid, password);

IPAddress ip(192,168,1,200);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
WiFi.config(ip, gateway, subnet);

while (WiFi.status() != WL_CONNECTED) 
{
  delay(200);
  Serial.print("*");
}
Serial.println("");
Serial.print("WiFi connected: ");
Serial.println(WiFi.localIP());

Serial.print("The URL to control ESP8266: ");

if (MDNS.begin("trafficlight")) // Start the mDNS responder for trafficlight.local
{
  Serial.println("http://trafficlight.local");
  MDNS.addService("http", "tcp", 80);
}else{
  Serial.println("Error setting up MDNS responder!");
}

server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

server.on("/RON", handleREDON);
server.on("/ROF", handleREDOFF);
server.on("/YON", handleYELLOWON);
server.on("/YOF", handleYELLOWOFF);
server.on("/GON", handleGREENON);
server.on("/GOF", handleGREENOFF);
server.on("/ALLOFF", handleALLOFF);

server.begin(); // Actually start the server

} // void setup()

void loop() 
{
  server.handleClient();
}

char * linkStr(int pin) {
    char buf[70] = "<a href=\""; //9
    if (pin == redPin) {
        if (digitalRead(pin) == HIGH) {
            strcat(buf, "ROF\" style=\"background-color:rgba("); // +34
            strcat(buf, "255,000,000,000,1.0)\"></a>"); // +26
        } else {
            strcat(buf, "RON\" style=\"background-color:rgba("); // +34
            strcat(buf, "255,000,000,000,0.1)\"></a>"); // +26
        }
    } else if (pin == yellowPin) {
        if (digitalRead(pin) == HIGH) {
            strcat(buf, "YOF\" style=\"background-color:rgba("); // +34
            strcat(buf, "255,165,000,000,1.0)\"></a>"); // +26
        } else {
            strcat(buf, "YON\" style=\"background-color:rgba("); // +34
            strcat(buf, "255,165,000,000,0.1)\"></a>"); // +26
        }
    } else if (pin == greenPin) {
        if (digitalRead(pin) == HIGH) {
            strcat(buf, "GOF\" style=\"background-color:rgba("); // +34
            strcat(buf, "000,128,000,000,1.0)\"></a>"); // +26
        } else {
            strcat(buf, "GON\" style=\"background-color:rgba("); // +34
            strcat(buf, "000,128,000,000,0.1)\"></a>"); // +26
        }
    }
    return buf;
}

void handleRoot() {
    char buf[438] = "<html><head><style>body{display:flex;flex-direction:column;align-items:center;}a{border-radius:50%;font-size:50px;width:400px;height:400px;}</style></head><body>"; //161

    strcat(buf, linkStr(redPin)); // +70
    strcat(buf, linkStr(yellowPin)); // +70
    strcat(buf, linkStr(greenPin)); // +70
    strcat(buf, "<a href=\"ALLOFF\" style=\"background-color:black;\"></a>"); // +53
    strcat(buf, "</body></html>"); // +14
    server.send(200, "text/html", buf);
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

// RED
void handleREDON(){
  Serial.println("REDON");
  server.send(200, "text/plain", "REDON");
  digitalWrite(redPin, HIGH);
}
void handleREDOFF(){
  Serial.println("REDOFF");
  server.send(200, "text/plain", "REDOFF");
  digitalWrite(redPin, LOW);
}

// YELLOW
void handleYELLOWON(){
  Serial.println("YELLOWON");
  server.send(200, "text/plain", "YELLOWON");
  digitalWrite(yellowPin, HIGH);
}
void handleYELLOWOFF(){
  Serial.println("YELLOWOFF");
  server.send(200, "text/plain", "YELLOWOFF");
  digitalWrite(yellowPin, LOW);
}

// GREEN
void handleGREENON(){
  Serial.println("GREENON");
  server.send(200, "text/plain", "GREENON");
  digitalWrite(greenPin, HIGH);
}
void handleGREENOFF(){
  Serial.println("GREENOFF");
  server.send(200, "text/plain", "GREENOFF");
  digitalWrite(greenPin, LOW);
}

// ALLOFF
void handleALLOFF(){
  Serial.println("ALLOFF");
  server.send(200, "text/plain", "ALLOFF");
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
}

