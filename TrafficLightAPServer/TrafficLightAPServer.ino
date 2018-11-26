#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>

#define AP_SSID "TrafficLight"
// AP IP: 192.168.4.1

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

int redPin = D4;  // also LED_BUILTIN on ESP8266
int yellowPin = D3;
int greenPin = D2;
int fourPin = D1;

int i = 0;

ESP8266WebServer server(80);//Service Port

void sendACK(){
  // send back a reply, to the IP address and port we got the packet from
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write("OK\n");
  Udp.endPacket();
}
void sendNACK(){
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write("NOK\n");
  Udp.endPacket();
}

void setup()
{
  Serial.begin(115200);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

  // must be set to AP, otherwise the STA can become unstable
  // and webserver/DHCP stop responding alltogether
  WiFi.mode(WIFI_AP);
  Serial.print("Setting soft-AP ... ");
  if(WiFi.softAP(AP_SSID) == true){
    Serial.println("Ready");
  }else{
    Serial.println("Failed!");
  }
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), localUdpPort);

  server.on("/", handleRoot);               // Call the 'handleRoot' function
  server.on("/REDON", handleREDON);
  server.on("/REDOFF", handleREDOFF);
  server.on("/YELLOWON", handleYELLOWON);
  server.on("/YELLOWOFF", handleYELLOWOFF);
  server.on("/GREENON", handleGREENON);
  server.on("/GREENOFF", handleGREENOFF);
  server.on("/ALLOFF", handleALLOFF);

  server.begin(); // Actually start the server
  Serial.printf("Web server: %s%s\n", "http://", WiFi.softAPIP().toString().c_str());
}

void loop()
{
  int packetSize = Udp.parsePacket();
  static int packetNumber=0;

  server.handleClient();
  
  if (packetSize)
  {
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    if(incomingPacket[len-1] == '\n'){ //get rid of '\n' for strcmp
      incomingPacket[len-1] = 0;
    }
    //Serial.printf("UDP packet contents: %s\n", incomingPacket);
    if(strcmp(incomingPacket, "REDON")==0){
      Serial.println("REDON");
      digitalWrite(redPin, HIGH);
      return;
    }
    if(strcmp(incomingPacket, "REDOFF")==0){
      Serial.println("REDOFF");
      digitalWrite(redPin, LOW);
      return;
    }
    if(strcmp(incomingPacket, "YELLOWON")==0){
      Serial.println("YELLOWON");
      digitalWrite(yellowPin, HIGH);
      return;
    }
    if(strcmp(incomingPacket, "YELLOWOFF")==0){
      Serial.println("YELLOWOFF");
      digitalWrite(yellowPin, LOW);
      return;
    }
    if(strcmp(incomingPacket, "GREENON")==0){
      Serial.println("GREENON");
      digitalWrite(greenPin, HIGH);
      return;
    }
    if(strcmp(incomingPacket, "GREENOFF")==0){
      Serial.println("GREENOFF");
      digitalWrite(greenPin, LOW);
      return;
    }
    if(strcmp(incomingPacket, "ALLOFF")==0){
      Serial.println("ALLOFF");
      digitalWrite(redPin, LOW);
      digitalWrite(yellowPin, LOW);
      digitalWrite(greenPin, LOW);
      return;
    }

    sendNACK();
    Serial.printf("%s %d\n", incomingPacket, packetNumber++);
  }
}

void handleRoot() {
  server.send(200, "text/html", "<a href=\"REDON\" style=\"font-size:70px\">REDON</a><br/>\
                                 <a href=\"REDOFF\" style=\"font-size:70px\">REDOFF</a><br/><br/>\
                                 <a href=\"YELLOWON\" style=\"font-size:70px\">YELLOWON</a><br/>\
                                 <a href=\"YELLOWOFF\" style=\"font-size:70px\">YELLOWOFF</a><br/><br/>\
                                 <a href=\"GREENON\" style=\"font-size:70px\">GREENON</a><br/>\
                                 <a href=\"GREENOFF\" style=\"font-size:70px\">GREENOFF</a><br/><br/>\
                                 <a href=\"ALLOFF\" style=\"font-size:50px\">ALLOFF</a>");
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

// RED
void handleREDON(){
  Serial.println("REDON");
  handleRoot();
  digitalWrite(redPin, HIGH);
}
void handleREDOFF(){
  Serial.println("REDOFF");
  handleRoot();
  digitalWrite(redPin, LOW);
}

// YELLOW
void handleYELLOWON(){
  Serial.println("YELLOWON");
  handleRoot();
  digitalWrite(yellowPin, HIGH);
}
void handleYELLOWOFF(){
  Serial.println("YELLOWOFF");
  handleRoot();
  digitalWrite(yellowPin, LOW);
}

// GREEN
void handleGREENON(){
  Serial.println("GREENON");
  handleRoot();
  digitalWrite(greenPin, HIGH);
}
void handleGREENOFF(){
  Serial.println("GREENOFF");
  handleRoot();
  digitalWrite(greenPin, LOW);
}

// ALLOFF
void handleALLOFF(){
  Serial.println("ALLOFF");
  handleRoot();
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
}
