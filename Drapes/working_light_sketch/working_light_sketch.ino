 #include <ESP8266WiFi.h>
 #include <WString.h>
 //the library are needed for autoconfig WiFi
 #include <DNSServer.h>
 #include <ESP8266WebServer.h>
 #include <WiFiManager.h>    

const char* ssid = "happy";
const char* password = "fourtwenty";
   
 // replace with your own API key and device ID,
 String apikey = "de3c2f8e-2d21-47cb-a91a-71f2c8245ae7";
 const char* deviceID="0200000255";
 const char* server = "www.linksprite.io";
 WiFiClient client;

 
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
 }

 
 void loop() {
   if (client.connect(server,80)) {  
   String  postStr ="{";
         postStr +="\"action\":\"query\",";
         postStr +="\"apikey\":\"";
         postStr += apikey;
         postStr +="\",";
         postStr +="\"deviceid\":\"";
         postStr += deviceID;
         postStr +="\",";
         postStr += "\"params\":";
         postStr += "[";
         postStr += "\"light\"";
         postStr +="]";
         postStr +="}";
    client.print("POST /api/http HTTP/1.1\n");
    client.print("Host: ");
    client.print(server);
    client.print("\nContent-Type: application/json\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);     
 }

 
 delay(1000);
 Serial.println("Store response...");
 String request = "";
 
 while (client.available()) {
   char c = client.read();
   request +=c;
 }
 
 if (request!= NULL){
   int index1 = request.indexOf(":{");
   int index2 = request.indexOf("},");
   String param = request.substring(index1, index2 + 1);
   Serial.print("The param is ");
   Serial.println(param);
   if(param.indexOf("off")>0){
       digitalWrite(BUILTIN_LED, HIGH);    
       Serial.println("OFF");
   } else if(param.indexOf("on")>0){
       digitalWrite(BUILTIN_LED, LOW);    
       Serial.println("ON");
   }
 client.stop();
 Serial.println("Waiting...");    
 delay(500);  
 }
 else{
  Serial.println("Response Null");
 }

 
}
