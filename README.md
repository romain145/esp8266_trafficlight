# esp8266_trafficlight
Control a esp8266 Traffic Light from your smartphone

# TrafficLightRx
This the Traffic Light Controller. It hosts a web server and receives commands from a web browser or a smartphone application and turns on/off the lights.
The commands are simple:
- REDON / REDOFF
- YELLOWON / YELLOWOFF
- GREENON / GREENOFF
- ALLOFF

The commands are simple url requests using HTTP_GET. There is no authentication/encryption. Note that the current implementation allows all of the 3 lights to be on at the same time (because why not?).

It can be accessed through mDNS at the local address: http://trafficlight.local. However, mDNS is not natively supported by Android, and it requires the "Bonjour" service on Windows to be enabled.

Based on the excellent tutorials:
- https://www.electronicshub.org/control-a-relay-using-esp8266-wifi-based
- https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html

Companion app:
- http://ai2.appinventor.mit.edu/?galleryId=5958187662573568


# TrafficLightTx
This is a simple transmitter to remotely control the Traffic Light without a smartphone or computer.
It has 3 inputs (one red, one yellow and one green if you didn't guess) and will send the appropriate command according to which input is set to 1 or 0. Simple :)

