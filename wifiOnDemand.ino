#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h> //ESP Web Server Library to host a web page

#define WifiPB D3

int led = D7;

ESP8266WebServer server(80);

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>Testing Wifi Config</h1><br>
Ciclk to turn <a href="ledOn" target="myIframe">ON</a><br>
Ciclk to turn <a href="ledOff" target="myIframe">OFF</a><br>
LED State:<iframe name="myIframe" width="250" height="100" frameBorder="0"><br>
<hr>
</center>

</body>
</html>
)=====";

void handleRoot() {
 Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  Serial.println("\n Starting");
  pinMode(WifiPB, INPUT);
  
 

  //If connection successful show IP address in serial monitor
 
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/ledOn", handleLedOn); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/ledOff", handleLedOff);
  

//  server.begin();                  //Start server
}

void handleLedOn(){
  Serial.println("LED ON");
  digitalWrite(led, HIGH);
  server.send(200, "text/html", "LED is ON"); //Send ADC value only to client ajax request
  }

void handleLedOff(){
  Serial.println("LED OFF");
  digitalWrite(led, LOW);
  server.send(200, "text/html", "LED is OFF"); //Send ADC value only to client ajax request
  }

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(WifiPB) == LOW){
  server.stop();
  WiFiManager wifiManager;
  if (!wifiManager.startConfigPortal("OnDemand High Voltages")){
    Serial.println("Timeout");
    delay(1000);
    ESP.reset();
    delay(1000);
    }
    Serial.print("Connected.... to ");
    Serial.println(WiFi.localIP());
    server.begin();
  }
  server.handleClient();

}
