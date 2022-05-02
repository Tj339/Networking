#include <Wire.h>
#include "Adafruit_TMP006.h"
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
//#include <string.h>


//Adafruit_TMP006 tmp006;
Adafruit_TMP006 tmp006(0x41);  // start with a diferent i2c address!
char ssid[] = "NETGEAR42";
char password[] = "dynamicplum866";

WiFiServer server(12004);

void setup() { 
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(PUSH1, INPUT);
  if (! tmp006.begin()) {
    Serial.println("No sensor found");
    while (1);
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.print("You're connected to ");
  Serial.println(ssid);
  while (WiFi.localIP() == INADDR_NONE) {
    Serial.print(".");
    delay(300);
  }
  printWifiStatus();
  Serial.println("Starting webserver on port 12004");
  server.begin();                           
  Serial.println("Webserver started!");
  
}


void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      int i = 0;
      char r[6] = "";
      while (client.available()) {
        r[i] = client.read();
        //Serial.write(r);
        i++;
      }  
      String h = r;
      //Serial.println(h);
      if (h == "START"){
        client.flush();
        while(currentLineIsBlank){
          delay(1);
          if (!client.connected()){
            //Serial.println("hi");
            break;
          }
          else{
              float Objt = tmp006.readObjTempC();
              int s = Objt*100;
              client.print(s);
              delay(1000);
         }
        }
      }
     /* else{
        client.println("Wrong");
      }*/
    }
    delay(1);
    Serial.print("\n");
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("Network Name: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
//Multithreading
