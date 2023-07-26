#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <stdio.h>

// WIFI Config
#define WIFI_SSID ""
#define WIFI_PASSWD " "

// MQTT Config
#define MQTT_HOST IPAddress(0,0,0,0)
#define MQTT_PORT 1883

// Topic Config
#define MQTT_SUB_COMMAND "esp/switch/command"
#define MQTT_PUB_STATUS "esp/switch/status"

// Init
WiFiClient espClient;
MQTTClient mqttClient;

#define RELAI_PIN D4

// function decleration
void connectToWifi();

void connectToMqttBroker();

void recievedMessage(String& topic, String& payload);


void setup() {
  pinMode(RELAI_PIN, OUTPUT);
  Serial.begin(9600);

  connectToWifi();
  connectToMqttBroker();
  mqttClient.onMessage(recievedMessage);
}

void loop() {
  mqttClient.loop();
  delay(10);
  
  if(!mqttClient.connected()){
    connectToMqttBroker();
  }
}

// function definition
void connectToWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  while (WiFi.status() != WL_CONNECTED) {
        Serial.println("Not Connected to WiFi... Trying to reconnect");
        delay(1000);
    }

  Serial.println("Connected to WiFi!");  
  Serial.println(WiFi.localIP());
}

void connectToMqttBroker(){
  mqttClient.begin(MQTT_HOST, espClient);

  Serial.print("\nconnecting to MQTT-Broker ...");
  while (!mqttClient.connect("esp8266", "USER", "PW")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Connection to MQTT-Broker established!");
  mqttClient.subscribe(MQTT_SUB_COMMAND);
}

void recievedMessage(String& topic, String& payload){
  if(topic=="esp/switch/command"){
    Serial.println("Setting relais to ");
    if(payload == "ON"){
      digitalWrite(RELAI_PIN,HIGH);
      Serial.println("ON");
    }
    else if(payload=="OFF"){
      digitalWrite(RELAI_PIN,LOW);
      Serial.println("OFF");
    }
  Serial.println("");
  }
}

