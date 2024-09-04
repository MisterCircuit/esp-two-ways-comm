#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "ESP8266-Access-Point";
const char* password = "123456789";

// Server URL to fetch temperature data
const char* serverNameTemp = "http://192.168.4.1/temperature";

unsigned long previousMillis = 0;
const long interval = 5000; // Interval at which to fetch data

void setup() {
  Serial.begin(9600);
  Serial.println();

  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      String temperature = httpGETRequest(serverNameTemp);
      Serial.println("Temperature: " + temperature + " ºC");

      // Save the last HTTP GET request time
      previousMillis = currentMillis;
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Specify the server URL
  http.begin(client, serverName);

  // Send HTTP GET request
  int httpResponseCode = http.GET();

  String payload = "--";

  if (httpResponseCode > 0) {
    payload = http.getString();
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
