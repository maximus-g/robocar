# ROBOCAR
## Simple wireless-controlled toy/robot car.  

The car can move forward or backward; turned right or left using a "remote controller".

The direction of the car is controlled using 4 momentary push buttons. The car is always at rest until any of the push buttons is pressed.

The direction or motion of the car is displayed to the user on the 16x2 Liquid Crystal Display (LCD).

When the distance between the car and an obstacle is less than 10 cm while it is moving forward or backward, the buzzer on the remote controller will ring and the car will immediately stop.

### **Components Used:**
* x2 Arduino Mega 2560 (You can use any type you want really, but the pins connection differ obviously);
* FT-MC-002 Kit;
* x2 NRF24L01 Transceiver Module (Download the driver at [link] https://github.com/nRF24/RF24);
* x2 HC-SR04 Ultrasonic Sensor;
* Liquid Crystal Display (LCD) 16x2;
* Piezoelectric Buzzer;
* x4 Momentary Push Buttons;
* x4 10k Resistors.

