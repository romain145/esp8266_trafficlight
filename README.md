# esp8266_trafficlight
Control a esp8266 Traffic Light from your smartphone

# TrafficLightRx
This the Traffic Light Controller. It receives commands from a web browser or a smartphone application, and turns on/off the lights.
The commands are simple:
- REDON / REDOFF
- YELLOWON / YELLOWOFF
- GREENON / GREENOFF

Based on the excellent tutorial:
https://www.electronicshub.org/control-a-relay-using-esp8266-wifi-based

Companion app:
http://ai2.appinventor.mit.edu/?galleryId=5958187662573568


# TrafficLightTx
This is a simple trnasmitter to remotely control the Traffic Light without a smartphone or computer.
It has 3 inputs (one red, one yellow and one green if you didn't guess) and will send the appropriate message according to which input is set to 1 or 0. Simple :)

