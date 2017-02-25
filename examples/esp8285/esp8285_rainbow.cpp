#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "credentials.h"
#include "my9291.h"

#define MY9291_DI_PIN       13
#define MY9291_DCKI_PIN     15
#define RAINBOW_DELAY       10

my9291 * _my9291;

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s\n", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println(" [WIFI] Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void rainbow(unsigned char index) {

    if (index < 85) {
        _my9291->setColor((my9291_color_t) { (unsigned int) index * 3, (unsigned int) 255 - index * 3, 0, 0 });
    } else if (index < 170) {
        index -= 85;
        _my9291->setColor((my9291_color_t) { (unsigned int) 255 - index * 3, 0, (unsigned int) index * 3, 0 });
    } else {
        index -= 170;
        _my9291->setColor((my9291_color_t) { 0, (unsigned int) index * 3, (unsigned int) 255 - index * 3, 0 });
    }

}

void setup() {

    // Init serial port and clean garbage
    Serial.begin(115200);
    Serial.println();
    Serial.println();

    // WiFi
    wifiSetup();

    // OTA
    ArduinoOTA.onStart([]() {
        Serial.printf("[OTA] Start\n");
    });

    ArduinoOTA.onEnd([]() {
        Serial.printf("\n[OTA] End\n");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("[OTA] Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("\n[OTA] Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.printf("Auth Failed\n");
        else if (error == OTA_BEGIN_ERROR) Serial.printf("Begin Failed\n");
        else if (error == OTA_CONNECT_ERROR) Serial.printf("Connect Failed\n");
        else if (error == OTA_RECEIVE_ERROR) Serial.printf("Receive Failed\n");
        else if (error == OTA_END_ERROR) Serial.printf("End Failed\n");
    });
    ArduinoOTA.begin();

    // MY9291
    _my9291 = new my9291(MY9291_DI_PIN, MY9291_DCKI_PIN, MY9291_COMMAND_DEFAULT);
    _my9291->setState(true);

}

void loop() {

    ArduinoOTA.handle();

    static unsigned char count = 0;
    static unsigned long last = millis();
    if (millis() - last > RAINBOW_DELAY) {
        last = millis();
        rainbow(count++);
    }

}
