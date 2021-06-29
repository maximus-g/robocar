
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define led 13
#include <Servo.h>

Servo lServo;
Servo rServo;

// Sensor
const int trigPin1 = 22;
const int echoPin1 = 24;
const int trigPin2 = 12;
const int echoPin2 = 9;
int distance1; // distance of the sensor
long duration1;
int distance2; // distance of the sensor
long duration2;

RF24 radio(7, 8); // CE, CSN

const byte addresses[][6] = {"00001", "00002"};
// const byte address[6] = "00001";
boolean state = 0;
int sensor_state1 = 0;
int sensor_state2 = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  // radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  // radio.startListening();

  // car wheels
  lServo.attach(11);
  rServo.attach(10);
  // LEDs
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  
}

void loop() {
  
  
  //delay(5);
  radio.startListening();
  
  if (radio.available()) {
    
    const int text;
    radio.read(&text, sizeof(text));

    // the car moves forward
    if (text == 2) 
    {
      digitalWrite(trigPin1, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin1, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin1, LOW);
      duration1 = pulseIn(echoPin1, HIGH);
      distance1 = duration1*0.034/2;
      
      if (distance1 > 10)
        {
          digitalWrite(2, HIGH);
          //delay(500);
          lServo.write(50);
          rServo.write(50);
          //Serial.println(text);
          //Serial.println();
          //Serial.println(distance);
        }
      else
        {
          digitalWrite(2, LOW);
          //delay(500);
          lServo.write(90);
          rServo.write(90);
          sensor_state1 = 1;
        }
    } 
    // the car moves backward
    else if (text == 3)
    {
      digitalWrite(trigPin2, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin2, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin2, LOW);
      duration2 = pulseIn(echoPin2, HIGH);
      distance2 = duration2*0.034/2;
      if (distance2 > 10)
        {
          digitalWrite(3, HIGH);
          //delay(500);
          lServo.write(180);
          rServo.write(180);
          //Serial.println(text);
          //Serial.println();
          //Serial.println(distance);
        }
      else
        {
          digitalWrite(3, LOW);
          //delay(500);
          lServo.write(90);
          rServo.write(90);
          sensor_state2 = 1;
       }
    }
    // the car moves right
    else if (text == 4)
    {
      digitalWrite(4, HIGH);
      //Serial.println(text);
      //Serial.println();
      //Serial.println(distance);
      lServo.write(75);
      rServo.write(105);
    }
    // the car moves left
     else if (text == 5)
    {
      digitalWrite(5, HIGH);
      //Serial.println(text);
      //Serial.println();
      //Serial.println(distance);
      lServo.write(105);
      rServo.write(75);
    }
    // the car stops
    else
    {
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      //Serial.println(text);
      //Serial.println();
      //Serial.println(distance);
      lServo.write(90);
      rServo.write(90);
      //delay(500);
    }

    delay(5);
    radio.stopListening();
    //Serial.println(distance);
    if(sensor_state1)
    {
      radio.write(&sensor_state1, sizeof(sensor_state1));
      //Serial.println(sensor_state1);
    }
    else
    {
      radio.write(&sensor_state2, sizeof(sensor_state2));
      //Serial.println(sensor_state2);
    }
    
    sensor_state1 = 0;
    sensor_state2 = 0;
    
  }
}
