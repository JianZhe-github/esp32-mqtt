#include "EspMQTTClient.h"
#include <ArduinoJson.h>

EspMQTTClient client(
  "", //wifiname
  "", //wifipassword
  "",  // MQTT Broker server ip
  "admin",   // Can be omitted if not needed
  "0000",   // Can be omitted if not needed
  "relay",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

void setup()
{
  Serial.begin(115200);
  pinMode(32,OUTPUT);

  // Optional functionalities of EspMQTTClient
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
  client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial(訂閱:精準位置)
  client.subscribe("ha/esp/relay", [](const String & payload) {
    Serial.println(payload);
    if (payload=="on")
    {
      digitalWrite(32,HIGH);
      client.publish("ha/esp/state/relay","on");
    }
    if (payload=="off")
    {
      digitalWrite(32,LOW);
      client.publish("ha/esp/state/relay","off");
    }
    
    
  });
  // Publish a message to "mytopic/test"(發布)
  client.publish("mytopic/test", "This is a message"); // You can activate the retain flag by setting the third parameter to true
}

void loop()
{
  client.loop();
}
