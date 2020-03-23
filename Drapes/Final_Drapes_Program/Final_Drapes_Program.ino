 #include <ESP8266WiFi.h>
 #include <WString.h>
 //the library are needed for autoconfig WiFi
 #include <DNSServer.h>
 #include <ESP8266WebServer.h>
 #include <WiFiManager.h>    
 #include <Servo.h>

 const char* ssid = "happy";
 const char* password = "fourtwenty";
 WiFiServer server(80);

 //Pin for servo white wire
 int servoPin = 3;
 //Pin for resetButton
 int button = 4;
 //Variable for 100 percent
 double fullRoll = 100;
 //Time it takes to go from 0 to 100
 unsigned long timeToOpenFully = 22000;
 //Current drape position
 double percentCurrent = 0;

 //Servo Object
 Servo servo;
 
 

 
void setup() {                
   Serial.begin(115200);
   delay(100);
   //Set up Servo and Button
   servo.attach(servoPin);
   pinMode(button, INPUT_PULLUP);
   
   Serial.print("Connecting to ");
   Serial.println(ssid);
  
   WiFi.begin(ssid, password);
  
   while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected");

   server.begin();
   Serial.println("Server Started");

   Serial.println(WiFi.localIP());
   
   
 }

void loop() {
  //If button is presed reset
  int sensorValue = digitalRead(button);
  if(sensorValue==LOW){
      reset();
  }
  
  //Check if client has connected
  WiFiClient client = server.available();
  if(!client) {
    return;
  }

  //*****************************************
  //Request Should Be                       *
  //IP/Get                                  *
  //or                                      *
  //IP/Set/Percent + "n"                    *  
  //*****************************************
  
  String req = client.readStringUntil('\r');
  //Serial.println(req);
  client.flush();

  
  String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";

//Get
  if(req.indexOf("Get")>0){
    response += "Drapes Are At ";
    response += percentCurrent;
    response += " Percent";
    response += "</html>\n";

  }
  
//Set
  else if(req.indexOf("Set")>0){
    int index2 = req.indexOf("n");
    if(index2>0){
      Serial.println(req);
      Serial.println(index2);
      String number = req.substring(9, index2);
      String movingDialog = response;
      movingDialog += "Drapes Moving";
      movingDialog += "</html>\n";      
      client.print(movingDialog);
      delay(1);
      client.flush();
      rotate(number.toInt());
      //Serial.print(number.toInt());
      response += "Drapes Moved To ";
      response += percentCurrent;
      response += " Percent";
      response += "</html>\n";      
    }
    else{
      response += "Where Would You Like To Move The Drapes To? ";
      response += "</html>\n";
    }
  }

//Edit
  else if(req.indexOf("Edit")>0){
    int index2 = req.indexOf("n");
    if(index2>0){
      String number = req.substring(10, index2);
      percentCurrent = number.toInt();
      response += "Drape Percent Set To ";
      response += percentCurrent;
      response += "</html>\n";  
    }
    else{
      response += "What Would You Like To Set The Percent To? ";
      response += "</html>\n";
  }   
}    

//Corner Case
  else{
    //Serial.println("Invalid Request Handled");
    response += "Invalid Request";
    response += "</html>\n";
  }

//Return Response
  //Serial.println("Response Sent");
  client.print(response);
  delay(1);
  client.stop();
  client.flush();
  //Serial.println("Client Disconnected");
  
  
}

boolean rotate(int percentTarget){
    //Sanity Check
    if(percentTarget>100){
      percentTarget=100;
    }
    else if(percentTarget<0){
      percentTarget=0;
    }
    else{
      //Do Nothing
    }

    
    //Get roll direction and time
    double rollDifference = percentTarget - percentCurrent;
    double absoluteRollDifferece = abs(rollDifference);
    //Serial.println((absoluteRollDifferece/100));
    
    //Get time
    unsigned long currentTime = millis();
    unsigned long targetTime = currentTime + (timeToOpenFully * (absoluteRollDifferece/100));
    Serial.write("Starting Rotation");
    //Start rotation
    if(rollDifference>0){
      //Roll Down
      servo.write(85);
      targetTime = targetTime + 150;
    }
    else if(rollDifference<0){
      //Roll Up
      servo.write(112);
    }
    else{
      //Stopped
      servo.write(99);
    }
    
    //Roll for correct amount of time
    while(currentTime<targetTime){
      currentTime = millis();
      delay(1);
    }
    Serial.write("\nStopping Rotation\n");
    //Stop Roll
    servo.write(99);
    
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
    delay(1);
  }
  
  //Stop servo
  servo.write(99);
  
}







