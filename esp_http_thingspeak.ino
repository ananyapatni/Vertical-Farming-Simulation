/*#include <Arduino.h>

 Wiring:
   ESP32 GPIO 17 (TX2) -> STM32 PB11 (RX)
   ESP32 GND           -> STM32 GND

#include <HTTPClient.h>

void uploadToThingSpeak(int waterLevel) {
  HTTPClient http;
  
  // The URL matches exactly what you used in Postman
  String url = "http://api.thingspeak.com/update?api_key=DXR3H6798TFD7X3L&field1=waterlevel" + String(waterLevel);
  
  http.begin(url);
  int httpCode = http.GET(); // ThingSpeak accepts GET for updates too!
  
  if (httpCode > 0) {
    Serial.println("Update successful, Code: " + String(httpCode));
  } else {
    Serial.println("Error on HTTP request");
  }
  
  http.end();
}
int tx_count = 0;

void setup() {
  Serial.begin(115200); // For PC Debugging
  
  // Initialize Serial2: Baud 115200, Pins 16(RX), 17(TX)
  // Even though we only transmit, we define both pins
  Serial2.begin(115200, SERIAL_8N1, 16, 17); 
  
  Serial.println("ESP32 Transmitter Started...");
}

void loop() {
  // Create the message
  String msg = "ESP_VAL:" + String(tx_count) + "\n";
  
  // Send to STM32
  Serial2.print(msg);
  
  // Mirror to PC Serial Monitor
  Serial.print("Sent to STM32: ");
  Serial.print(msg);

  tx_count++;
  if(tx_count > 100) tx_count = 0; // Reset after 100

  delay(2000); // Wait 2 seconds
}
*/

/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-post-ifttt-thingspeak-arduino/ 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ananya's phone";
const char* password = "qwerty123";

// Domain Name with full URL Path for HTTP POST Request
const char* serverName = "http://api.thingspeak.com/update";
// Service API Key
String apiKey = "DXR3H6798TFD7X3L";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Set timer to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Timer set to 10 seconds (10000)
unsigned long timerDelay = 10000;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");

  // Random seed is a number used to initialize a pseudorandom number generator
  randomSeed(analogRead(33));
}

void loop() {
  //Send an HTTP POST request every 10 seconds
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
    
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      
      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      String httpRequestData = "api_key=" + apiKey + "&field1=" + String(random(40));           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);
      
      /*
      // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      // JSON data to send with HTTP POST
      String httpRequestData = "{\"api_key\":\"" + apiKey + "\",\"field1\":\"" + String(random(40)) + "\"}";           
      // Send HTTP POST request
      int httpResponseCode = http.POST(httpRequestData);*/
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
