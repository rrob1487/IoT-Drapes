// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 3; 

int spin = 0;

unsigned long previous = 0;
const long interval = 2000;

// Create a servo object 
Servo Servo1; 
void setup() { 
   // We need to attach the servo to the used pin number 
   //Serial.begin(9600);
   Servo1.attach(servoPin); 
   changeSpin();
}
void loop(){ 
   unsigned long current = millis();
   if(current - previous >= interval){
    previous = current;
    changeSpin();
   }
}

void changeSpin(){
  int i = spin;
  if(i == 0){
    // Make servo go CounterClockwise
     //Serial.print(0);
     //Serial.print("\n");
     Servo1.write(90);
     Servo1.write(180);
     spin = 1;
  }
  else{
     //Make Servo go Clockwise
     //Serial.print(1);
     //Serial.print("\n");
     Servo1.write(90);
     Servo1.write(0);
     spin = 0;
  }
}

