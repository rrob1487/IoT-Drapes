 #include <ESP8266WiFi.h>
 #include <WString.h>
 //the library are needed for autoconfig WiFi
 #include <DNSServer.h>
 #include <ESP8266WebServer.h>
 #include <WiFiManager.h>    

const char* ssid = "GOAWAY";
const char* password = "04110340041103400411034004";
   
 // replace with your own API key and device ID,
 String apikey = "de3c2f8e-2d21-47cb-a91a-71f2c8245ae7";
 const char* deviceID="0200000255";
 //IPAddress addr = 10.11.12.13;

 //WiFiServer server(addr, 80) would set custom address if i knew how to define an ip -_-
 WiFiServer server(80);
 int val = 90;

 
 void setup() {                
   Serial.begin(115200);
   pinMode(BUILTIN_LED, OUTPUT);
   
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

  if(req.indexOf("Get")>0){
    response += "Drapes Are At ";
    response += val;
    response += " Percent";
    response += "</html>\n";

  }
  else if(req.indexOf("Set")>0){
    //Serial.println("Changing Drapes to: ");
    int index2 = req.indexOf("n");
    String number = req.substring(9, index2);
    val = number.toInt();
    //Serial.print(val);
    response += "Drapes Moving To ";
    response += val;
    response += " Percent";
    response += "</html>\n";
  }
  else{
    //Serial.println("Invalid Request Handled");
    client.stop();
    client.flush();
    return;
  }

  //Return Response
  //Serial.println("Response Sent");
  client.print(response);
  delay(1);
  client.stop();
  client.flush();
  //Serial.println("Client Disconnected");
  
  
}










