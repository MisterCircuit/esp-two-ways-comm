#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Set your access point network credentials
const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

String readTemp() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  return String(temperatureC);
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(9600);
  Serial.println();

  // Start the DS18B20 sensor
  sensors.begin();

  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Route to read temperature data
//  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
//    sensors.requestTemperatures(); 
//    float temperatureC = sensors.getTempCByIndex(0);
//    String temperatureString = String(temperatureC);
//    request->send_P(200, "text/plain", temperatureString.c_str());
//    Serial.println("Temperature Sent: " + temperatureString + "ºC");
//  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTemp().c_str());
  });


  // Start server
  server.begin();
}

void loop() {
  // No need to put anything in loop for this basic server setup
}
