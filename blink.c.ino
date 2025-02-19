#include <WiFi.h>
#include <WebServer.h>
#include "config.h" 

WebServer server(80);  // Start web server on port 80
const int lightPin = 2; // Change this if using a relay

// HTML Page with ON/OFF Buttons
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Light Control</title>
    <style>
        body { text-align: center; font-family: Arial, sans-serif; }
        h1 { color: #333; }
        button { font-size: 20px; padding: 10px; margin: 10px; width: 150px; }
        .on { background: green; color: white; }
        .off { background: red; color: white; }
    </style>
</head>
<body>
    <h1>ESP32 Light Control</h1>
    <button class="on" onclick="toggleLight('on')">TURN ON</button>
    <button class="off" onclick="toggleLight('off')">TURN OFF</button>

    <script>
        function toggleLight(state) {
            fetch('/light?state=' + state);
        }
    </script>
</body>
</html>
)rawliteral";

// Handle root URL
void handleRoot() {
    server.send(200, "text/html", htmlPage);
}

// Handle light control
void handleLight() {
    if (server.hasArg("state")) {
        String state = server.arg("state");

        if (state == "on") {
            digitalWrite(lightPin, HIGH);
            Serial.println("Light ON");
            server.send(200, "text/plain", "Light turned ON");
        } else if (state == "off") {
            digitalWrite(lightPin, LOW);
            Serial.println("Light OFF");
            server.send(200, "text/plain", "Light turned OFF");
        } else {
            server.send(400, "text/plain", "Invalid state");
        }
    } else {
        server.send(400, "text/plain", "Bad Request");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(lightPin, OUTPUT);
    digitalWrite(lightPin, LOW); // Ensure the light is OFF at startup

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nConnected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Define server routes
    server.on("/", handleRoot);
    server.on("/light", handleLight);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
}
