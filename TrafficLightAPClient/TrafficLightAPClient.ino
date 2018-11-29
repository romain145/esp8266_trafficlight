#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "TrafficLight";

#define serverIP  "192.168.4.1"
#define serverPort 4210

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

int redPin = D4;  // also LED_BUILTIN on ESP8266
int yellowPin = D3;
int greenPin = D2;
int fourPin = D1;

int red_prev = 0;
int yellow_prev = 0;
int green_prev = 0;

int i = 0;

void sendPacket(char* data)
{
  Udp.beginPacket(serverIP, serverPort);
  Udp.write(data);
  if(Udp.endPacket()==0){
    Serial.println("Error pk");
    while(1); // WDT restart in case of error
  }
}

void setup() 
{
Serial.begin(115200);

pinMode(redPin, INPUT_PULLUP);
pinMode(yellowPin, INPUT_PULLUP);
pinMode(greenPin, INPUT_PULLUP);

Serial.print("Connecting to: ");
Serial.println(ssid);

WiFi.begin(ssid);

while (WiFi.status() != WL_CONNECTED) 
{
  delay(200);
  Serial.print("*");
}
Serial.println("");
Serial.print("Connected to: ");
Serial.println(serverIP);

Udp.begin(localUdpPort);
Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
delay(2000); // Delay before sending

} // void setup()

void loop()
{
  int packetSize;

  packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Udp.read(incomingPacket, 255);
    //Serial.printf("%s %d\n", incomingPacket, packetNumber++);
  }

  // RED
  if(red_prev == 0 && !digitalRead(redPin))
  {
    red_prev = 1;
    Serial.println("RON");
    sendPacket("RON");
  }
  if(red_prev == 1 && digitalRead(redPin))
  {
    red_prev = 0;
    Serial.println("ROF");
    sendPacket("ROF");
  }
  
  // YELLOW
  if(yellow_prev == 0 && !digitalRead(yellowPin))
  {
    yellow_prev = 1;
    Serial.println("YON");
    sendPacket("YON");
  }
  if(yellow_prev == 1 && digitalRead(yellowPin))
  {
    yellow_prev = 0;
    Serial.println("YOF");
    sendPacket("YOF");
  }
  
  // GREEN
  if(green_prev == 0 && !digitalRead(greenPin))
  {
    green_prev = 1;
    Serial.println("GON");
    sendPacket("GON");
  }
  if(green_prev == 1 && digitalRead(greenPin))
  {
    green_prev = 0;
    Serial.println("GOF");
    sendPacket("GOF");
  }
  delay(10);

} // end loop
