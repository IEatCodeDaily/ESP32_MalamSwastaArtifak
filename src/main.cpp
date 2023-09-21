#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

const char *ssid = "Sudarshana-Chakra";
const char *password = "AkuBukanKrishna";

const char *expectedPassword = "anagatateknikfisikaduaribusembilanbelas";

AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);


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