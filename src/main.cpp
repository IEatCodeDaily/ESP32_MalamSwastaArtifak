#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char *ssid = "MahaPadma";
const char *password = "nawasenaduaribuduapuluhdua";

const char *expectedPassword = "anagatateknikfisikaduaribusembilanbelas";


const int red_pin = 12;
const int green_pin = 14;
const int blue_pin = 13;

const int freq = 5000;
const int resolution = 8;
const int red_channel = 0;
const int green_channel = 1;
const int blue_channel = 2;


AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);

    // Configure LED PWM functionalities
    ledcSetup(red_channel, freq, resolution);
    ledcSetup(green_channel, freq, resolution);
    ledcSetup(blue_channel, freq, resolution);

    // Attach the channel to the GPIO
    ledcAttachPin(red_pin, red_channel);
    ledcAttachPin(green_pin, green_channel);
    ledcAttachPin(blue_pin, blue_channel);

    if (!SPIFFS.begin(true)) {
        Serial.println("An error occurred while mounting SPIFFS");
        return;
    }

    Serial.println("SPIFFS mounted");


    IPAddress apIP(133, 22, 19, 1);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(ssid, password);

    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 60);
    Serial.println("Access Point mode activated");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/login.html", "text/html");
        ledcWrite(0, 255);
        ledcWrite(1, 255);
        ledcWrite(2, 200);
        //Serial.println("GET REQUEST ON /");
    });

    server.on("/check", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("password", true)) {
            String userPassword = request->getParam("password", true)->value();
            if (userPassword == expectedPassword) {
                request->send(SPIFFS, "/mantra.html", "text/html");
                ledcWrite(0, 255);
                ledcWrite(1, 30);
                ledcWrite(2, 0);
            } else {
                request->send(SPIFFS, "/login.html", "text/html");
                ledcWrite(0, 255);
                ledcWrite(1, 0);
                ledcWrite(2, 0);
            }
        } else {
            request->send(SPIFFS, "/login.html", "text/html");
        }
    });

    server.on("/rgb", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Check if the URL has the "color" query parameter
        if (request->hasParam("color")) {
            // Get the value of the "color" query parameter
            String colorValue = request->getParam("color")->value();
            
            // Split the colorValue into individual RGB components
            int red, green, blue;
            if (sscanf(colorValue.c_str(), "%d,%d,%d", &red, &green, &blue) == 3) {
                // Here, you can set the RGB LED duty cycle based on the RGB values
                // (You need to implement this part according to your hardware setup)

                // Send a response indicating that the RGB values have been set
                request->send(200, "text/plain", "RGB values set to: " + colorValue);
                ledcWrite(0, red);
                ledcWrite(1, green);
                ledcWrite(2, blue);
            } else {
                // If parsing fails, send an error response
                request->send(400, "text/plain", "Invalid 'color' format");
            }
        } else {
            // If the "color" query parameter is missing, send an error response
            request->send(400, "text/plain", "Missing 'color' query parameter");
        }
    });

    server.on("/qr.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/qr.png", "image/png");
        });
    
    server.begin();
    
}

void loop() {
}