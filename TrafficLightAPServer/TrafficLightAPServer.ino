#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#define AP_SSID "TrafficLight"
// AP IP: 192.168.4.1

const IPAddress apIp(192, 168, 4, 1);
DNSServer dnsServer;

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets

int redPin = D4;  // also LED_BUILTIN on ESP8266
int yellowPin = D3;
int greenPin = D2;
int fourPin = D1;

int i = 0;

ESP8266WebServer server(80);//Service Port

void sendACK() {
  // send back a reply, to the IP address and port we got the packet from
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write("OK\n");
  Udp.endPacket();
}
void sendNACK() {
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

  WiFi.persistent(false);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_AP);
  Serial.print("Setting soft-AP ... ");
  WiFi.softAPConfig(apIp, apIp, IPAddress(255, 255, 255, 0));
  if (WiFi.softAP(AP_SSID) == true) {
    Serial.println("Ready");
  } else {
    Serial.println("Failed!");
  }

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", apIp);

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), localUdpPort);

  server.on("/", handleRoot);               // Call the 'handleRoot' function
  server.on("/RON", handleREDON);
  server.on("/ROF", handleREDOFF);
  server.on("/YON", handleYELLOWON);
  server.on("/YOF", handleYELLOWOFF);
  server.on("/GON", handleGREENON);
  server.on("/GOF", handleGREENOFF);
  server.on("/ALLOFF", handleALLOFF);
  server.onNotFound(httpDefault);

  server.begin(); // Actually start the server
  Serial.printf("Web server: %s%s\n", "http://", WiFi.softAPIP().toString().c_str());
}

void loop()
{
  int packetSize;
  static int packetNumber = 0;
  static int nbStationConnected = 0;

  dnsServer.processNextRequest();

  if (nbStationConnected != WiFi.softAPgetStationNum()) {
    nbStationConnected = WiFi.softAPgetStationNum();
    Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
  }

  server.handleClient();

  packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    if (incomingPacket[len - 1] == '\n') { //get rid of '\n' for strcmp
      incomingPacket[len - 1] = 0;
    }
    //Serial.printf("UDP packet contents: %s\n", incomingPacket);
    if (strcmp(incomingPacket, "RON") == 0) {
      Serial.println("RON");
      digitalWrite(redPin, HIGH);
      return;
    }
    if (strcmp(incomingPacket, "ROF") == 0) {
      Serial.println("ROF");
      digitalWrite(redPin, LOW);
      return;
    }
    if (strcmp(incomingPacket, "YON") == 0) {
      Serial.println("YON");
      digitalWrite(yellowPin, HIGH);
      return;
    }
    if (strcmp(incomingPacket, "YOF") == 0) {
      Serial.println("YOF");
      digitalWrite(yellowPin, LOW);
      return;
    }
    if (strcmp(incomingPacket, "GON") == 0) {
      Serial.println("GON");
      digitalWrite(greenPin, HIGH);
      return;
    }
    if (strcmp(incomingPacket, "GOF") == 0) {
      Serial.println("GOF");
      digitalWrite(greenPin, LOW);
      return;
    }
    if (strcmp(incomingPacket, "ALLOFF") == 0) {
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

char * linkStr(int pin) {
  char buf[66] = "<a href=\""; //9
  if (pin == redPin) {
    if (digitalRead(pin) == HIGH) {
      strcat(buf, "ROF\" style=\"background-color:rgba("); // +34
      strcat(buf, "255,000,000,1.0)\"></a>"); // +22
    } else {
      strcat(buf, "RON\" style=\"background-color:rgba("); // +34
      strcat(buf, "255,000,000,0.1)\"></a>"); // +22
    }
  } else if (pin == yellowPin) {
    if (digitalRead(pin) == HIGH) {
      strcat(buf, "YOF\" style=\"background-color:rgba("); // +34
      strcat(buf, "255,165,000,1.0)\"></a>"); // +22
    } else {
      strcat(buf, "YON\" style=\"background-color:rgba("); // +34
      strcat(buf, "255,165,000,0.1)\"></a>"); // +22
    }
  } else if (pin == greenPin) {
    if (digitalRead(pin) == HIGH) {
      strcat(buf, "GOF\" style=\"background-color:rgba("); // +34
      strcat(buf, "000,128,000,1.0)\"></a>"); // +22
    } else {
      strcat(buf, "GON\" style=\"background-color:rgba("); // +34
      strcat(buf, "000,128,000,0.1)\"></a>"); // +22
    }
  }
  return buf;
}

void
httpDefault()
{
  server.sendHeader("Location", "http://trafficlight.pinball/", true);
  server.send(302, "text/plain", "");
  server.client().stop();
}

void handleRoot() {
  char buf[436] = "<html><head><style>body{display:flex;flex-direction:column;align-items:center;}a{border-radius:50%;font-size:50px;width:30vw;height:30vw;}</style></head><body>"; //159

  strcat(buf, linkStr(redPin)); // +70
  strcat(buf, linkStr(yellowPin)); // +70
  strcat(buf, linkStr(greenPin)); // +70
  strcat(buf, "<a href=\"ALLOFF\" style=\"background-color:black;\"></a>"); // +53
  strcat(buf, "</body></html>"); // +14
  server.send(200, "text/html", buf);
}

//void handleNotFound(){
//  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
//}

// RED
void handleREDON() {
  Serial.println("RON");
  digitalWrite(redPin, HIGH);
  handleRoot();
}
void handleREDOFF() {
  Serial.println("ROF");
  digitalWrite(redPin, LOW);
  handleRoot();
}

// YELLOW
void handleYELLOWON() {
  Serial.println("YON");
  digitalWrite(yellowPin, HIGH);
  handleRoot();
}
void handleYELLOWOFF() {
  Serial.println("YOF");
  digitalWrite(yellowPin, LOW);
  handleRoot();
}

// GREEN
void handleGREENON() {
  Serial.println("GON");
  digitalWrite(greenPin, HIGH);
  handleRoot();
}
void handleGREENOFF() {
  Serial.println("GOF");
  digitalWrite(greenPin, LOW);
  handleRoot();
}

// ALLOFF
void handleALLOFF() {
  Serial.println("ALLOFF");
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
  handleRoot();
}
