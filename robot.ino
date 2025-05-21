/*VR408
 * Copyright (C) 2014 Velleman nv
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 *
 * Note:
 * -----
 * for use on Arduino Uno  board, set jumpers to UNO  on VRSSM Shield!
 * for use on Arduino Mega board, set jumpers to Mega on VRSSM Shield!
 * 
 */

/*
 * Modified by Morten Barthel, 2025
 * See LICENSE for details.
 */

#include <Servo.h>                        // The ALLBOT library needs the servo.h library
#include <ALLBOT.h>                       // Do not forget to include the ALLBOT library, download it from the manuals.velleman.eu website

ALLBOT BOT(8);                            // Number of motors

enum MotorName {                          
  hipFrontLeft,
  hipFrontRight,
  hipRearLeft,
  hipRearRight,
  kneeFrontLeft,
  kneeFrontRight,
  kneeRearLeft,
  kneeRearRight
};

long randNumber0;                         // This variable is used for random function
long randNumber1;                         // This variable is used for random function
int sounderPin = 13;                      // Declaring what pin the sounder on the VRSSM is connected to

String command;  // Global variable that stores part of the decoded IR command
int commandCode = 0; // For selected Command
int times;                                // Global variable that stores part the received IR command
int speedms;                              // Global variable that stores part the received IR command

boolean userControl = false;                 // Set this to true if you want to control it manually

// ESP32 Communication vars
#define SPEED_PIN A0
#define TIMES_PIN A1
// Commands via binary
#define COMMAND_PIN0 0 // Accept or deny commands
#define COMMAND_PIN1 1 // +1
#define COMMAND_PIN2 2 // +2
#define COMMAND_PIN3 3 // +4
#define COMMAND_PIN4 4 // +8


void setup() 
{ 
  // ESP32
  pinMode(SPEED_PIN, INPUT);
  pinMode(TIMES_PIN, INPUT);
  pinMode(COMMAND_PIN0, INPUT);
  pinMode(COMMAND_PIN1, INPUT);
  pinMode(COMMAND_PIN2, INPUT);
  pinMode(COMMAND_PIN3, INPUT);  
  pinMode(COMMAND_PIN4, INPUT);

  // NAME.attach(motorname, pin, init-angle, flipped, offset-angle);
  
  BOT.attach(hipFrontLeft,   A1,  45, 0, 0); //for Arduino Uno pin A1        // for Arduino Mega pin 24 = SV3
  BOT.attach(hipFrontRight,  A0,  45, 1, 0); //for Arduino Uno pin A0        // for Arduino Mega pin 31 = SV10
  BOT.attach(hipRearLeft,     9,  45, 1, 0); //for Arduino Uno pin 9 = SV9   // for Arduino Mega pin 36 = SV15
  BOT.attach(hipRearRight,    4,  45, 0, 0); //for Arduino Uno pin 4 = SV4   // for Arduino Mega pin 49 = SV28
 
  BOT.attach(kneeFrontLeft,  11,  45, 1, 0); //for Arduino Uno pin 11 = SV11 // for Arduino Mega pin 23 = SV2
  BOT.attach(kneeFrontRight,  2,  45, 0, 0); //for Arduino Uno pin 2  = SV2  // for Arduino Mega pin 32 = SV11
  BOT.attach(kneeRearLeft,   10,  45, 1, 0); //for Arduino Uno pin 10 = SV10 // for Arduino Mega pin 35 = SV14
  BOT.attach(kneeRearRight,   3,  45, 0, 0); //for Arduino Uno pin 3  = SV3  // for Arduino Mega pin 50 = SV29

  // INIT sounder
  pinMode(sounderPin, OUTPUT);

  // Wait for joints to be initialized
  delay(500);
  
  // INIT the random seed, this is used to create random actions
  randomSeed(analogRead(5));

  // Chirp for ready
  chirp(1, 50);
  chirp(1, 255);
  chirp(3, 0);

}

void loop()                               // Main program loop
{
  
  if (userControl == true)                  // Choose between user commands or random action
    {
      getcommand();                       // Listen for command
      executecommand();                   // Execute any receveid commands
    }
    else
    {
      randNumber0 = random(0, 18);        
      randNumber1 = random(0, 20);
      delay(randNumber1*100);
      
      switch (randNumber0) {              // This is the random action case
        case 0:
          randNumber0 = random(10, 255);
          leanforward(randNumber0);
        break;
        case 1:
          randNumber0 = random(10, 255);
          leanbackward(randNumber0);
        break;
        case 2:
          randNumber0 = random(10, 255);
          leanleft(randNumber0);
        break;
        case 3:
          randNumber0 = random(10, 255);
          leanright(randNumber0);
        break;
        case 4:
          randNumber0 = random(10, 255);
          lookleft(randNumber0);
        break;
        case 5:
          randNumber0 = random(10, 255);
          lookright(randNumber0);
        break;
        case 6:
          randNumber0 = random(10, 255);
          randNumber1 = random(1, 5);
          turnleft(randNumber1, randNumber0);
        break;
        case 7:
          randNumber0 = random(10, 255);
          randNumber1 = random(1, 5);
          turnright(randNumber1, randNumber0);
        break;
        case 8:
          randNumber0 = random(10, 150);
          randNumber1 = random(2, 10);
          walkforward(randNumber1, randNumber0);
        break;
        case 9:
          randNumber0 = random(10, 150);
          randNumber1 = random(2, 10);
          walkbackward(randNumber1, randNumber0);
        break;
        case 10:
          randNumber0 = random(10, 150);
          randNumber1 = random(2, 10);
          walkleft(randNumber1, randNumber0);
        break;
        case 11:
          randNumber0 = random(10, 150);
          randNumber1 = random(2, 10);
          walkright(randNumber1, randNumber0);
        break;
        case 12:
          randNumber0 = random(5, 30);
          randNumber1 = random(2, 10);
          scared(randNumber1, randNumber0);
        break;
        case 13:
          randNumber0 = random(0, 255);
          randNumber1 = random(1, 30);
          chirp(randNumber1, randNumber0);
        break;
        case 14:
          randNumber0 = random(25, 100);
          randNumber1 = random(1, 5);
          wavefrontright(randNumber1, randNumber0);
        break;
        case 15:
          randNumber0 = random(25, 100);
          randNumber1 = random(1, 5);
          wavefrontleft(randNumber1, randNumber0);
        break;
        case 16:
          randNumber0 = random(25, 100);
          randNumber1 = random(1, 5);
          waverearright(randNumber1, randNumber0);
        break;
        case 17:
          randNumber0 = random(25, 100);
          randNumber1 = random(1, 5);
          waverearleft(randNumber1, randNumber0);
        break;
      }
   }
}
//--------------------------------------------------------------
int readCommandCode() {
  return (digitalRead(COMMAND_PIN1) << 0) |
         (digitalRead(COMMAND_PIN2) << 1) |
         (digitalRead(COMMAND_PIN3) << 2) |
         (digitalRead(COMMAND_PIN4) << 3);
}

void getcommand (void) {
  if (digitalRead(COMMAND_PIN0)) { // Only accept commands when enabled
    commandCode = readCommandCode(); // 0-15
    times = analogRead(TIMES_PIN) / 100; // scale 0–10
    speedms = analogRead(SPEED_PIN) / 4; // scale 0–255

    switch (commandCode) {
      case 0: command = "WF"; break;
      case 1: command = "WB"; break;
      case 2: command = "WL"; break;
      case 3: command = "WR"; break;
      case 4: command = "TL"; break;
      case 5: command = "TR"; break;
      case 6: command = "LF"; break;
      case 7: command = "LB"; break;
      case 8: command = "LL"; break;
      case 9: command = "LR"; break;
      case 10: command = "FL"; break;
      case 11: command = "FR"; break;
      case 12: command = "RL"; break;
      case 13: command = "RR"; break;
      case 14: command = "SC"; break;
      case 15: command = "CH"; break;
      default: command = ""; break;
    }
  }
}

//--------------------------------------------------------------

void executecommand (void)                // Execute the commands that are stored in the global vars.
{ 

  // 16 commands
  if (command == "WF")
  {
    walkforward(times, (speedms*5));
  }
  else if (command == "WB")
  {
    walkbackward(times, (speedms*5));
  }
  else if (command == "WL")
  {
    walkleft(times, (speedms*5));
  }
  else if (command == "WR")
  {
    walkright(times, (speedms*5));
  }
  else if (command == "TR")
  {
    turnright(times, (speedms*5));
  }
  else if (command == "TL")
  {
    turnleft(times, (speedms*5));
  }
  else if (command == "LF")
  {
    leanforward(speedms*5);
  }
  else if (command == "LB")
  {
    leanbackward(speedms*5);
  }
  else if (command == "LL")
  {
    leanleft(speedms*5);
  }
  else if (command == "LR")
  {
    leanright(speedms*5);
  }
  else if (command == "FR")
  {
    wavefrontright(times, speedms*5);
  }
  else if (command == "FL")
  {
    wavefrontleft(times, speedms*5);
  }
  else if (command == "RR")
  {
    waverearright(times, speedms*5);
  }
  else if (command == "RL")
  {
    waverearleft(times, speedms*5);
  }
  else if (command == "SC")
  {
    scared(times, speedms);
  }
  else if (command == "CH")
  {
    chirp(times, speedms);
  }
}


//--------------------------------------------------------------
// Movements
//--------------------------------------------------------------
void chirp(int beeps, int speedms){

  for (int i = 0; i < beeps; i++){
    for (int i = 0; i < 255; i++){
      digitalWrite(sounderPin, HIGH);
      delayMicroseconds((355-i)+ (speedms*2));
      digitalWrite(sounderPin, LOW);
      delayMicroseconds((355-i)+ (speedms*2));
    }
     delay(30);
  }
}
//--------------------------------------------------------------
void waverearleft(int waves, int speedms){

    BOT.move(kneeRearLeft, 180);
    BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++){
    BOT.move(hipRearLeft, 0);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 65);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 0);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 45);
    BOT.animate(speedms);
  }    
  
    BOT.move(kneeRearLeft, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void waverearright(int waves, int speedms){

    BOT.move(kneeRearRight, 180);
    BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++){
    BOT.move(hipRearRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 65);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 45);
    BOT.animate(speedms);
  }    
  
    BOT.move(kneeRearRight, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void wavefrontright(int waves, int speedms){

    BOT.move(kneeFrontRight, 180);
    BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++){
    BOT.move(hipFrontRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 65);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
  }    
  
    BOT.move(kneeFrontRight, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void wavefrontleft(int waves, int speedms){

    BOT.move(kneeFrontLeft, 180);
    BOT.animate(speedms);
    
  for (int i = 0; i < waves; i++){
    BOT.move(hipFrontLeft, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 65);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 0);
    BOT.animate(speedms);
    
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
  }    
  
    BOT.move(kneeFrontLeft, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void scared(int shakes, int beeps){
    BOT.move(kneeFrontRight, 0);
    BOT.move(kneeRearRight, 0);
    BOT.move(kneeFrontLeft, 0);
    BOT.move(kneeRearLeft, 0);
    BOT.animate(50);
    
    for (int i = 0; i < shakes; i++){
 
      BOT.move(hipRearRight, 80);
      BOT.move(hipRearLeft, 10);
      BOT.move(hipFrontRight, 10);
      BOT.move(hipFrontLeft, 80);
      BOT.animate(100);
      
      BOT.move(hipRearLeft, 80);
      BOT.move(hipRearRight, 10);
      BOT.move(hipFrontLeft, 10);
      BOT.move(hipFrontRight, 80);
      BOT.animate(50);
    }
    
    BOT.move(hipRearRight, 45);
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(200);
    
    chirp(beeps, 0);
    
    BOT.move(kneeFrontRight, 45);
    BOT.move(kneeRearRight, 45);
    BOT.move(kneeFrontLeft, 45);
    BOT.move(kneeRearLeft, 45);
    BOT.animate(75);
}
//--------------------------------------------------------------
void leanright(int speedms){
    BOT.move(kneeFrontRight, 90);
    BOT.move(kneeRearRight, 90);
    BOT.animate(speedms);
    
    delay(speedms/2);
    
    BOT.move(kneeFrontRight, 45);
    BOT.move(kneeRearRight, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void leanleft(int speedms){
    BOT.move(kneeFrontLeft, 90);
    BOT.move(kneeRearLeft, 90);
    BOT.animate(speedms);
    
    delay(speedms/2);
    
    BOT.move(kneeFrontLeft, 45);
    BOT.move(kneeRearLeft, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void leanforward(int speedms){
    BOT.move(kneeFrontLeft, 90);
    BOT.move(kneeFrontRight, 90);
    BOT.animate(speedms);
    
    delay(speedms/2);
    
    BOT.move(kneeFrontLeft, 45);
    BOT.move(kneeFrontRight, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void leanbackward(int speedms){
    BOT.move(kneeRearLeft, 90);
    BOT.move(kneeRearRight, 90);
    BOT.animate(speedms);
    
    delay(speedms/2);
    
    BOT.move(kneeRearLeft, 45);
    BOT.move(kneeRearRight, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void lookleft(int speedms){
    BOT.move(hipRearLeft, 80);
    BOT.move(hipRearRight, 10);
    BOT.move(hipFrontLeft, 10);
    BOT.move(hipFrontRight, 80);
    BOT.animate(speedms);
    
    delay(speedms/2);
    
    BOT.move(hipRearRight, 45);
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void lookright(int speedms){
    BOT.move(hipRearRight, 80);
    BOT.move(hipRearLeft, 10);
    BOT.move(hipFrontRight, 10);
    BOT.move(hipFrontLeft, 80);
    BOT.animate(speedms);
    
    delay(speedms/2);
    
    BOT.move(hipRearRight, 45);
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
}
//--------------------------------------------------------------
void walkforward(int steps, int speedms){
  for (int i = 0; i < steps; i++){
    BOT.move(kneeRearRight, 80);
    BOT.move(kneeFrontLeft, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 80);
    BOT.move(hipFrontLeft, 20);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 30);
    BOT.move(kneeFrontLeft, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearRight, 45);
    BOT.move(kneeFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 80);
    BOT.move(kneeFrontRight, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 80);
    BOT.move(hipFrontRight, 20);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 30);
    BOT.move(kneeFrontRight, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 45);
    BOT.move(kneeFrontRight, 45);
    BOT.animate(speedms);
  }
}
//--------------------------------------------------------------
void walkbackward(int steps, int speedms){
  for (int i = 0; i < steps; i++){
    BOT.move(kneeRearRight, 80);
    BOT.move(kneeFrontLeft, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 20);
    BOT.move(hipFrontLeft, 80);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 30);
    BOT.move(kneeFrontLeft, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearRight, 45);
    BOT.move(kneeFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 80);
    BOT.move(kneeFrontRight, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 20);
    BOT.move(hipFrontRight, 80);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 30);
    BOT.move(kneeFrontRight, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 45);
    BOT.move(kneeFrontRight, 45);
    BOT.animate(speedms);
  }
}
//--------------------------------------------------------------
void walkleft(int steps, int speedms){
  for (int i = 0; i < steps; i++){
    BOT.move(kneeRearRight, 80);
    BOT.move(kneeFrontLeft, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 0);
    BOT.move(hipFrontLeft, 90);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 30);
    BOT.move(kneeFrontLeft, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearRight, 45);
    BOT.move(kneeFrontLeft, 45);
    BOT.animate(speedms);
     
    BOT.move(kneeRearLeft, 80);
    BOT.move(kneeFrontRight, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 90);
    BOT.move(hipFrontRight, 0);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 30);
    BOT.move(kneeFrontRight, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 45);
    BOT.move(kneeFrontRight, 45);
    BOT.animate(speedms);
  }
}
//--------------------------------------------------------------
void walkright(int steps, int speedms){
  for (int i = 0; i < steps; i++){
    BOT.move(kneeRearLeft, 80);
    BOT.move(kneeFrontRight, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 0);
    BOT.move(hipFrontRight, 90);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 30);
    BOT.move(kneeFrontRight, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 45);
    BOT.move(kneeFrontRight, 45);
    BOT.animate(speedms);
     
    BOT.move(kneeRearRight, 80);
    BOT.move(kneeFrontLeft, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 90);
    BOT.move(hipFrontLeft, 0);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 30);
    BOT.move(kneeFrontLeft, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearRight, 45);
    BOT.move(kneeFrontLeft, 45);
    BOT.animate(speedms);
  }
}
//--------------------------------------------------------------
void turnleft(int steps, int speedms){
  for (int i = 0; i < steps; i++){
    BOT.move(kneeRearRight, 80);
    BOT.move(kneeFrontLeft, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 90);
    BOT.move(hipFrontLeft, 90);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 30);
    BOT.move(kneeFrontLeft, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearRight, 45);
    BOT.move(kneeFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 80);
    BOT.move(kneeFrontRight, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 0);
    BOT.move(hipFrontRight, 0);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 30);
    BOT.move(kneeFrontRight, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 45);
    BOT.move(kneeFrontRight, 45);
    BOT.animate(speedms);
  }
}
//--------------------------------------------------------------
void turnright(int steps, int speedms){
  for (int i = 0; i < steps; i++){
    BOT.move(kneeRearRight, 80);
    BOT.move(kneeFrontLeft, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 0);
    BOT.move(hipFrontLeft, 0);
    BOT.animate(speedms);

    BOT.move(kneeRearRight, 30);
    BOT.move(kneeFrontLeft, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearRight, 45);
    BOT.move(hipFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearRight, 45);
    BOT.move(kneeFrontLeft, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 80);
    BOT.move(kneeFrontRight, 80);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 90);
    BOT.move(hipFrontRight, 90);
    BOT.animate(speedms);

    BOT.move(kneeRearLeft, 30);
    BOT.move(kneeFrontRight, 30);
    BOT.animate(speedms);
    
    BOT.move(hipRearLeft, 45);
    BOT.move(hipFrontRight, 45);
    BOT.animate(speedms);
    
    BOT.move(kneeRearLeft, 45);
    BOT.move(kneeFrontRight, 45);
    BOT.animate(speedms);
  }
}
