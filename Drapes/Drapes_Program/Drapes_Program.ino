#include <Servo.h>

//Pin for servo white wire
int servoPin = 3;
//Pin for resetButton
int button = 4;
//Variable for 100 percent
double fullRoll = 100;
//Time it takes to go from 0 to 100
unsigned long timeToOpenFully = 18000;
//Current drape position
double percentCurrent = 0;

//Servo Object
Servo servo;

void setup() {
  //Set up Servo and Button
  servo.attach(servoPin);
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  //If button is presed reset
   int sensorValue = digitalRead(button);
   if(sensorValue==LOW){
      reset();
  }

  
}


boolean rotate(double percentTarget){
    //Get roll direction and time
    double rollDifference = percentTarget - percentCurrent;
    double absoluteRollDifferece = abs(rollDifference);
    
    //Get time
    unsigned long currentTime = millis();
    unsigned long targetTime = currentTime + (timeToOpenFully * (absoluteRollDifferece/100));
    
    //Start rotation
    if(rollDifference>0){
      //Clockwise
      servo.write(0);
    }
    else if(rollDifference<0){
      //Counter Clockwise
      servo.write(180);
    }
    else{
      //Stopped
      servo.write(90);
    }

    //Roll for correct amount of time
    while(currentTime<targetTime){
      currentTime = millis();
    }
    
    //Stop Roll
    servo.write(90);
    
    //Set Current Percent
    percentCurrent = percentTarget;
    
    return true;
}

void reset(){
  //Clear current
  percentCurrent = 0;

  //Get button state
  int sensorValue = digitalRead(button);
  
  //Start servo wind up
  servo.write(180);
  
  //Wait until button is released
  while(sensorValue == LOW){
    sensorValue = digitalRead(button);
  }
  
  //Stop servo
  servo.write(92);
  
}

