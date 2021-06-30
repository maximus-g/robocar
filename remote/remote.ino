
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>                        // includes the LiquidCrystal Library

LiquidCrystal lcd(31, 32, 34, 35, 36, 37);



RF24 radio(7, 8);                                 // CE, CSN

const int buzzer = 13;                            //buzzer to arduino pin 9
const byte addresses[][6] = {"00001", "00002"};
const int forward = 2;                            // forward direction
const int backward = 3;                           // backward direction
const int right = 4;                              // right direction
const int left = 5;                               // left direction
boolean state = 0;                                // direction
int sensor_state1 = 0;                            // led state


void setup()
{
  Serial.begin(9600);
  radio.begin();
  lcd.begin(16,2);
  radio.openWritingPipe(addresses[1]);            // 00002
  radio.openReadingPipe(1, addresses[0]);         // 00001
  radio.setPALevel(RF24_PA_MIN);
  pinMode(2, INPUT);                              // forward pb
  pinMode(3, INPUT);                              // backward pb
  pinMode(4, INPUT);                              // right pb
  pinMode(5, INPUT);                              // left pb
  pinMode(buzzer, OUTPUT);
}

void loop() {

  delay(5);

  radio.stopListening();


  // move the car forward
  if(digitalRead(forward) == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Direction:");
    lcd.setCursor(0,1);
    lcd.print("Forward!");
    const int text = 2;
    radio.write(&text, sizeof(text));
  }

  // move the car backward
  else if(digitalRead(backward) == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Direction:");
    lcd.setCursor(0,1);
    lcd.print("Backward!");
    const int text = 3;
    radio.write(&text, sizeof(text));
  }

  // move the car to the right
  else if(digitalRead(right) == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Direction:");
    lcd.setCursor(0,1);
    lcd.print("Right!");
    const int text = 4;
    radio.write(&text, sizeof(text));
  }

  // move the car to the left
  else if(digitalRead(left) == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Direction:");
    lcd.setCursor(0,1);
    lcd.print("Left!");
    const int text = 5;
    radio.write(&text, sizeof(text));
  }

  // stop the car
  else
  {
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("STATIONARY");
    const int text = 0;
    radio.write(&text, sizeof(text));
  }
  

  radio.startListening();                             // This sets the module as receiver
  while(!radio.available());
  radio.read(&sensor_state1, sizeof(sensor_state1));  // Reading the data
  Serial.println(sensor_state1);
  
  if (sensor_state1 == 1)
  {
    tone(buzzer, 1000);                               // Send 1KHz sound signal...
    delay(70);
    noTone(buzzer);                                   // Stop sound...
    delay(70);     
  }
  else
  {
    noTone(buzzer);
  }
}
