#include <WiFi.h>
#include <WebServer.h>
#include "config.h"


WebServer server(80); // Start web server on port 80

// Handle root URL
void handleRoot() {
  server.send(200, "text/plain", "Send data to /receive via POST");
}

// Handle data reception
void handleReceive() {
  if (server.hasArg("plain")) { // Check if data is received in raw format
    String data = server.arg("plain");
    Serial.println("Data received: " + data);
    server.send(200, "application/json", "{\"status\":\"Data received successfully\"}");
  } else {
    server.send(400, "application/json", "{\"error\":\"No data received\"}");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define server routes
  server.on("/", handleRoot);
  server.on("/receive", HTTP_POST, handleReceive);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // Process incoming client requests
}
