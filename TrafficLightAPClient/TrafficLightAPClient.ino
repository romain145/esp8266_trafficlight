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
  int packetSize = Udp.parsePacket();
  char packet[10];
  static int packetNumber=0;

  if (packetSize)
  {
    Udp.read(incomingPacket, 255);
    //Serial.printf("%s %d\n", incomingPacket, packetNumber++);
  }

  //for(packetNumber=0; packetNumber<100; packetNumber++){
  Udp.beginPacket(serverIP, serverPort);
  itoa(packetNumber, packet, 10);
  Udp.write(packet);
  if(Udp.endPacket()==0){
    Serial.printf("Error pk %d\n", packetNumber);
    while(1); // WDT restart in case of error
  }
  Serial.printf("pk %d\n", packetNumber++);
  delay(10);
  //}
  //delay(5000);
}
