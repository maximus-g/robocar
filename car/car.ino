
/* RF module libraries */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/* Servo Motor library */
#include <Servo.h>

#define led 13

/* Declaring servos */
Servo lServo;
Servo rServo;

/* Create the NRF24 object, CE and CSN pins to set up the module to be on standby or active mode */
RF24 radio(7, 8);

/* Declaring the rest of the pins */
const byte addresses[][6] = {"00001", "00002"};

/* Delcaring ultrasonic sensor variable pins */
const int trigPin1 = 22;                      // Trig pin for the front-sensor
const int echoPin1 = 24;                      // Echo pin for the front-sensor
const int trigPin2 = 12;                      // Trig pin for the back-sensor
const int echoPin2 = 9;                       // Echo pin for the back-sensor
int distance1;                                // Distance between the front-sensor and an obstacle
long duration1;                               // Travel time
int distance2;                                // Distance between the back-sensor and an obstacle
long duration2;                               // Travel time


int sensor_state1 = 0;
int sensor_state2 = 0;

void setup() {
  radio.begin();
  radio.openWritingPipe(addresses[0]);        // 00001 as the sending address
  radio.openReadingPipe(1, addresses[1]);     // 00002 as the receiving address
  radio.setPALevel(RF24_PA_MIN);              

  /* Car DC motor gear box pins */
  lServo.attach(11);
  rServo.attach(10);

  /* Setting up ultrasonic sensors' pins */
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT); 
}

void loop() {
  delay(5);


  /************ RECEIVING MODE *****************/
  
  radio.startListening();                       // Sets up the module to be a receiving

  /* Read if there's incoming data */
  if (radio.available())
  {
    const int dir;                              // Direction variable
    radio.read(&dir, sizeof(dir));              // Read incoming direction

    /* The car moves forward */
    if (dir == 2) 
    {
      /* Calculate the distance between the car and the obstacle */
      digitalWrite(trigPin1, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin1, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin1, LOW);
      duration1 = pulseIn(echoPin1, HIGH);
      distance1 = duration1*0.034/2;               // Calculated distance

      /* If the distance between the car and an obstacle is greater 10 cm, the cars moves forward */
      if (distance1 > 10)
        {
          lServo.write(50);
          rServo.write(50);
        }
      /* Otherwise it stops */
      else
        {
          lServo.write(90);
          rServo.write(90);
          sensor_state1 = 1;                           // This value is going to be sent back to the remote
        }
    } 
    /* The car moves backward */
    else if (dir == 3)
    {
      /* Calculate the distance between the car and the obstacle */
      digitalWrite(trigPin2, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin2, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin2, LOW);
      duration2 = pulseIn(echoPin2, HIGH);
      distance2 = duration2*0.034/2;                     // Calculated distance
      /* If the distance between the car and an obstacle is greater than 10 cm, the cars reverses */
      if (distance2 > 10)
        {
          lServo.write(180);
          rServo.write(180);
        }
     /* Otherwise it stops */
      else
        {
          lServo.write(90);
          rServo.write(90);
          sensor_state2 = 1;                              // This value is going to be sent back to the remote
       }
    }
    /* The car moves right */
    else if (dir == 4)
    {

      lServo.write(75);
      rServo.write(105);
    }
    /* The car moves left */
     else if (dir == 5)
    {
      lServo.write(105);
      rServo.write(75);
    }
    /* The car stops */
    else
    {
      lServo.write(90);
      rServo.write(90);
    }

    delay(5);

    /************ RECEIVING MODE *****************/
    
    radio.stopListening();                            // Sets up the module to be a sender

    /* If this sensor-state for the front-sensor is 1, send back to the remote */
    if(sensor_state1)
    {
      radio.write(&sensor_state1, sizeof(sensor_state1));
    }
    /* If this sensor-state for the back-sensor is 1, send back to the remote */
    else
    {
      radio.write(&sensor_state2, sizeof(sensor_state2));
    }

    /* Resets sensor_states */
    sensor_state1 = 0;
    sensor_state2 = 0;
  }
}