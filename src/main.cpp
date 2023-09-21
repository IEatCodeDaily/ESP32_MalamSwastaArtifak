#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char *ssid = "Sudarshana-Chakra";
const char *password = "AkuBukanKrishna";

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

    Serial.println("Access Point mode activated");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Access Point IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/login.html", "text/html");
        //Serial.println("GET REQUEST ON /");
    });

    server.on("/check", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("password", true)) {
            String userPassword = request->getParam("password", true)->value();
            if (userPassword == expectedPassword) {
                request->send(SPIFFS, "/mantra.html", "text/html");
            } else {
                request->send(SPIFFS, "/login.html", "text/html");
            }
        } else {
            request->send(SPIFFS, "/login.html", "text/html");
        }
    });
    server.begin();
    
}

void loop() {
}