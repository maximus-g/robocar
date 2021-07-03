
/* RF module libraries */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/* LCD library */
#include <LiquidCrystal.h>


/* Create the NRF24 object, CE and CSN pins to set up the module to be on standby or active mode */
RF24 radio(7, 8);
/* Declaring LCD pins */
LiquidCrystal lcd(31, 32, 34, 35, 36, 37);

/* Declaring the rest of the pins */
const byte addresses[][6] = {"00001", "00002"};

int forward = 2;                                // forward direction
int backward = 3;                               // backward direction
int right = 4;                                  // right direction
int left = 5;                                   // left direction

const int buzzer = 13;                          // buzzer on pin 13

boolean state = 0;                              
int sensor_state1 = 0;                          // led state
int sensor_state2 = 0;                          // led state


void setup() {
  radio.begin();                                // Initialize the RF module
  lcd.begin(16,2);                              // Initialize the LCD                              
  radio.openWritingPipe(addresses[1]);          // 00002 as the sending address
  radio.openReadingPipe(1, addresses[0]);       // 00001 as the receiving address
  radio.setPALevel(RF24_PA_MIN);                //
  pinMode(forward, INPUT);                      // forward pb
  pinMode(backward, INPUT);                     // backward pb
  pinMode(right, INPUT);                        // right pb
  pinMode(left, INPUT);                         // left pb
  pinMode(buzzer, OUTPUT);                      // Setup the buzzer pin as an output
}

void loop() {
  delay(5);                                     // Delay for 5 milli-sec

  
  /********* SENDING MODE ******************/
  
  radio.stopListening();                        // Sets up the module to be a sender
  /* Move the car forward */
  if(digitalRead(forward) == HIGH)
  {
    lcd.clear();                                // Clear LCD
    lcd.setCursor(0,0);                         // Set LCD cursor to the col 0 row 0
    lcd.print("Direction:");                    // Write "Direction:" on the LCD
    lcd.setCursor(0,1);                         // Set LCD cursor to the col 0 row 1
    lcd.print("Forward!");                      // Write "Forward:" on the LCD                      
    const int dir = 2;                          // Set direction to 2 
    radio.write(&dir, sizeof(dir));             // Send direction to the car using the RF module
  }
  /* Move the car backward */
  else if(digitalRead(backward) == HIGH)
  {
    lcd.clear();                                // Clear LCD
    lcd.setCursor(0,0);                         // Set LCD cursor to the col 0 row 0
    lcd.print("Direction:");                    // Write "Direction:" on the LCD
    lcd.setCursor(0,1);                         // Set LCD cursor to the col 0 row 1                   
    lcd.print("Backward!");                     // Write "Backward:" on the LCD
    const int dir = 3;                          // Set direction to 3
    radio.write(&dir, sizeof(dir));             // Send direction to the car using the RF module
  }
  /* Move the car right */
  else if(digitalRead(right) == HIGH)
  {
    lcd.clear();                                // Clear LCD
    lcd.setCursor(0,0);                         // Set LCD cursor to the col 0 row 0
    lcd.print("Direction:");                    // Write "Direction:" on the LCD
    lcd.setCursor(0,1);                         // Set LCD cursor to the col 0 row 1                   
    lcd.print("Right!");                        // Write "Right:" on the LCD
    const int dir = 4;                          // Set direction to 4
    radio.write(&dir, sizeof(dir));             // Send direction to the car using the RF module
  }
  /* The car moves left */
  else if(digitalRead(left) == HIGH)
  {
    lcd.clear();                                // Clear LCD
    lcd.setCursor(0,0);                         // Set LCD cursor to the col 0 row 0
    lcd.print("Direction:");                    // Write "Direction:" on the LCD
    lcd.setCursor(0,1);                         // Set LCD cursor to the col 0 row 1                   
    lcd.print("Left!");                         // Write "Left:" on the LCD
    const int dir = 5;                          // Set direction to 5
    radio.write(&dir, sizeof(dir));             // Send direction to the car using the RF module
  }
  /* Stop the car */
  else
  {
    lcd.clear();                                // Clear LCD
    lcd.setCursor(3,0);                         // Set LCD cursor to the col 3 row 0
    lcd.print("STATIONARY");                    // Write "STATIONARY" on the LCD
    const int dir = 0;                          // Set direction to 5
    radio.write(&dir, sizeof(dir));             // Send 0 to the car using the RF module
  }

  /********* RECEIVING MODE ******************/
  
  radio.startListening();                       // Sets up the module to be a receiver
  while(!radio.available());                    // Read data for as long as there's available incoming data
  
  radio.read(&sensor_state1, sizeof(sensor_state1)); // Reading data from the front sensor
  radio.read(&sensor_state2, sizeof(sensor_state2)); // Reading data from the back sensor
  
  if (sensor_state1 == 1)
  {
    tone(buzzer, 1000);                         // Send 1KHz sound signal
    delay(70);                                  // Delay for 70 mS
    noTone(buzzer);                             // Stop sound
    delay(70);                                  // Delay for 70 mS
  }
  else if (sensor_state2 == 1)
  {
    tone(buzzer, 1000);                         // Send 1KHz sound signal
    delay(70);                                  // Delay for 70 mS
    noTone(buzzer);                             // Stop sound
    delay(70);
  }
  else
  {
    noTone(buzzer);
  }
}