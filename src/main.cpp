// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include <Arduino.h>
#include <EEPROM.h>

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#include "webpage.h"

#define RELE_PIN D1
#define SWITCH_PIN D8

#define ADDR_STATUS 0
#define ADDR_SWITCH 1
#define ADDR_WIFI 2
#define STATUS_RELE_ON 1
#define STATUS_RELE_OFF 0
#define SWITCH_ENABLED 1
#define SWITCH_DISABLED 0

#define WiFI_Fallback_PSK "qwerty123456"

ESP8266WebServer server(80);

char light_status = 0;
char switch_status = 1;
char status_changed = 0;
char fallback = 0;
char switch_pressed = 0;

struct WiFiData {
    char ssid[100];
    char password[100];
    char mdns[30];
} wifi_data;

void setSwitchStatus(char status) {
    switch_status = status;
    EEPROM.write(ADDR_SWITCH, status);
    EEPROM.commit();
}

void updateConfig() {
    String newSSID, newPSK;

    newSSID = server.arg("SSID");
    newPSK = server.arg("PSK");

    Serial.print("New SSID: ");
    Serial.print(newSSID);
    Serial.print(" New PSK: ");
    Serial.println(newPSK);

    strcpy(wifi_data.ssid, newSSID.c_str());
    strcpy(wifi_data.password, newPSK.c_str());

    EEPROM.put(ADDR_WIFI, wifi_data);
    EEPROM.commit();

}

void updateMdnsConfig() {
    String newMDNS = server.arg("mdns");

    Serial.print("New mDNS name: ");
    Serial.println(newMDNS);

    strcpy(wifi_data.mdns, newMDNS.c_str());

    EEPROM.put(ADDR_WIFI, wifi_data);
    EEPROM.commit();
}

void configWebServer() {
    server.on("/", []() {
        if (light_status == STATUS_RELE_ON) {
            if (switch_status == SWITCH_ENABLED) {
                server.send(200, "text/html", page_on_switch_on);
            } else {
                server.send(200, "text/html", page_on_switch_off);
            }
        } else {
            if (switch_status == SWITCH_ENABLED) {
                server.send(200, "text/html", page_off_switch_on);
            } else {
                server.send(200, "text/html", page_off_switch_off);
            }
        }

        if (server.hasArg("SSID") && server.hasArg("PSK")) {
            updateConfig();
        }

        if (server.hasArg("mdns")) {
            updateMdnsConfig();
        }

    });

    server.on("/web_off", []() {
        status_changed = 1;
        light_status = STATUS_RELE_OFF;
        Serial.println("Web Command: Turn Off.");
        if (switch_status == SWITCH_ENABLED) {
            server.send(200, "text/html", page_off_switch_on);
        } else {
            server.send(200, "text/html", page_off_switch_off);
        }
    });

    server.on("/web_on", []() {
        status_changed = 1;
        light_status = STATUS_RELE_ON;
        Serial.println("Web Command: Turn on.");
        if (switch_status == SWITCH_ENABLED) {
            server.send(200, "text/html", page_on_switch_on);
        } else {
            server.send(200, "text/html", page_on_switch_off);
        }
    });

    server.on("/web_enableswitch", []() {
        setSwitchStatus(1);
        Serial.println("Web Command: Enable Switch.");
        if (light_status == STATUS_RELE_ON) {
            server.send(200, "text/html", page_on_switch_on);
        } else {
            server.send(200, "text/html", page_off_switch_on);
        }
    });

    server.on("/web_disableswitch", []() {
        setSwitchStatus(0);
        Serial.println("Web Command: Disable Switch.");
        if (light_status == STATUS_RELE_ON) {
            server.send(200, "text/html", page_on_switch_off);
        } else {
            server.send(200, "text/html", page_off_switch_off);
        }
    });

    server.on("/off", []() {
        status_changed = 1;
        light_status = STATUS_RELE_OFF;
        Serial.println("Web Command: Turn Off.");
        server.send(200, "text/html", "off");

    });

    server.on("/on", []() {
        status_changed = 1;
        light_status = STATUS_RELE_ON;
        Serial.println("Web Command: Turn on.");
        server.send(200, "text/html", "on");
    });

    server.on("/enableswitch", []() {
        setSwitchStatus(1);
        Serial.println("Web Command: Enable Switch.");
        server.send(200, "text/html", "enableswitch");
    });

    server.on("/disableswitch", []() {
        setSwitchStatus(0);
        Serial.println("Web Command: Disable Switch.");
        server.send(200, "text/html", "disableswitch");
    });

    server.on("/name", []() {
        setSwitchStatus(0);
        Serial.println("Web Command: Disable Switch.");
        server.send(200, "text/html", wifi_data.mdns);
    });

    server.on("/light_status", []() {
        Serial.println("Web Command: Status.");
        if(light_status == STATUS_RELE_ON){
            server.send(200, "text/html", "on");
        }else {
            server.send(200, "text/html", "off");
        }
    });

    server.on("/switch_status", []() {
        Serial.println("Web Command: switch_status.");
        if(switch_status == 1){
            server.send(200, "text/html", "enabled");
        }else {
            server.send(200, "text/html", "disabled");
        }
    });

    server.on("/config", []() { server.send(200, "text/html", page_config); });

    server.begin();
}

void setupWiFiAP() {
    WiFi.mode(WIFI_AP);

    // Do a little work to get a unique-ish name. Append the
    // last two bytes of the MAC (HEX'd) to "Thing-":
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                   String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
    macID.toUpperCase();
    String AP_NameString = "SmartLight " + macID;

    char AP_NameChar[AP_NameString.length() + 1];
    memset(AP_NameChar, 0, AP_NameString.length() + 1);

    for (int i = 0; i < AP_NameString.length(); i++)
        AP_NameChar[i] = AP_NameString.charAt(i);

    WiFi.softAP(AP_NameChar, WiFI_Fallback_PSK);

    Serial.printf("Setting up access point with SSID: %s, PSK: %s", AP_NameChar, WiFI_Fallback_PSK);
}

void connectWiFi() {
    WiFi.begin(wifi_data.ssid, wifi_data.password);

    // Wait for connection
    uint8_t i = 0;

    // Wait 20 seconds to connect
    while (WiFi.status() != WL_CONNECTED && i < 40) {
        delay(500);
        Serial.print(".");
        i++;
    }

    // If I can't connect to the wifi I become the Access point to allow to change my config.
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(wifi_data.ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }else {
        setupWiFiAP();
        fallback = 1;
    }
}

void setLightStatus(char status) {
    light_status = status;
    EEPROM.write(ADDR_STATUS, status);
    EEPROM.commit();

    if (status == STATUS_RELE_ON) {
        digitalWrite(RELE_PIN, LOW);
        Serial.println("Light On.");
    } else {
        digitalWrite(RELE_PIN, HIGH);
        Serial.println("Light Off.");
    }
}

void configMDNS() {
    if (!MDNS.begin(wifi_data.mdns)) {
        Serial.println("Error setting up MDNS responder!");
        while (1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");

    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
}

void recovery_poweroff() {
    if (switch_status == 1) {
        if (light_status == STATUS_RELE_OFF) {
            setLightStatus(STATUS_RELE_ON);
        } else {
            setLightStatus(STATUS_RELE_OFF);
        }
    } else {
        if (light_status == STATUS_RELE_OFF) {
            setLightStatus(STATUS_RELE_OFF);
        } else {
            setLightStatus(STATUS_RELE_ON);
        }
    }
}

void setup() {
    Serial.begin(9600);

    pinMode(RELE_PIN, OUTPUT);
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    EEPROM.begin(512);

    EEPROM.get(ADDR_WIFI, wifi_data);

    light_status = EEPROM.read(ADDR_STATUS);
    switch_status = EEPROM.read(ADDR_SWITCH);

    Serial.print("Previus state of the rele: ");
    Serial.println(light_status);

    setLightStatus(light_status);

    // Uncoment the following line if you use the loss of power to switch the status os the light.
    // recovery_poweroff();

    connectWiFi();
    configWebServer();
    configMDNS();
}

void toggleLight() {
    if (light_status == STATUS_RELE_ON) {
        setLightStatus(STATUS_RELE_OFF);
    }else{
        setLightStatus(STATUS_RELE_ON);
    }
}

void loop() {
    if (status_changed == 1) {
        setLightStatus(light_status);
        status_changed = 0;
    }

    char switch_read = digitalRead(SWITCH_PIN);
    if (switch_read == HIGH && switch_pressed == false) {
        toggleLight();
        switch_pressed = true;
    }

    if (switch_read == LOW) {
        switch_pressed = false;
    }

    server.handleClient();
}
