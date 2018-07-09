
// RXI: GPIO1
// LED: GPIO2
// TXO: GPIO3

#include <ESP8266WiFi.h>

const char* ssid = "ssid";
const char* password = "password";

int redPin = 3;
int yellowPin = LED_BUILTIN; // GPIO2 of ESP8266
int greenPin = 4;

int i = 0;

WiFiServer ESPserver(80);//Service Port

void setup() 
{
Serial.begin(115200);
pinMode(redPin, OUTPUT);
pinMode(yellowPin, OUTPUT);
digitalWrite(yellowPin, HIGH); // led off
pinMode(greenPin, OUTPUT);

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

//IPAddress ip(192,168,1,254);   
//IPAddress gateway(192,168,1,1);   
//IPAddress subnet(255,255,255,0);   
//WiFi.config(ip, gateway, subnet);
//delay(5000);

while (WiFi.status() != WL_CONNECTED) 
{
  delay(100);
  Serial.print("*");
}
Serial.println("");
Serial.println("WiFi connected");

// Start the server
ESPserver.begin();
Serial.println("Server started");

// Print the IP address
Serial.print("The URL to control ESP8266: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
}

void loop() 
{
// Check if a client has connected
WiFiClient client = ESPserver.available();
if (!client) 
{
  return;
}

// Wait until the client sends some data
Serial.println("New Client");
i=0;
while(!client.available())
{
  delay(100);
  if(i++>10)
  {
    Serial.println("Client Timeout");
    client.stop();
    return;
  }
}

// Read the first line of the request
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();

client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); //  IMPORTANT
//client.println("<!DOCTYPE HTML>");
//client.println("<html>");

// RED
if (request.indexOf("/REDON") != -1) 
{
  Serial.println("REDON");
  client.print("REDON");
  digitalWrite(redPin, LOW);
} 
if (request.indexOf("/REDOFF") != -1)
{
  Serial.println("REDOFF");
  client.print("REDOFF");
  digitalWrite(redPin, HIGH);
}

// YELLOW
if (request.indexOf("/YELLOWON") != -1) 
{
  Serial.println("YELLOWON");
  client.print("YELLOWON");
  digitalWrite(yellowPin, LOW);
} 
if (request.indexOf("/YELLOWOFF") != -1)
{
  Serial.println("YELLOWOFF");
  client.print("YELLOWOFF");
  digitalWrite(yellowPin, HIGH);
}

// GREEN
if (request.indexOf("/GREENON") != -1) 
{
  Serial.println("GREENON");
  client.print("GREENON");
  digitalWrite(greenPin, LOW);
} 
if (request.indexOf("/GREENOFF") != -1)
{
  Serial.println("GREENOFF");
  client.print("GREENOFF");
  digitalWrite(greenPin, HIGH);
}

delay(1);
//client.stop();
Serial.println("Client disconnected");
Serial.println("");
}
