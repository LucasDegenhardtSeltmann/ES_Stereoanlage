#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string>

const char* ssid = "Hauskeller";
const char* password = "(harmantund$choen";
const char* MQTT_BROKER = "192.168.178.34";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
byte i2c_rcv; 

void setupWifi();
void reconnect();
void receiveEvent(int howMany);
void requestEvent();

void setup() {
  Serial.begin(9600);  // start serial for output
  Serial.print("\nStartvorgang beendet");
  Wire.begin(D1,D2);

  setupWifi();
  client.setServer(MQTT_BROKER, 1884);
}

void loop() {

  if (!client.connected()) {
		reconnect();
	}
	client.loop();
  /* An Arduino senden
   Wire.beginTransmission(8); 
   Wire.write("Hello Arduino"); 
   Wire.endTransmission();    
  */
  // Request data from slave.
  Wire.beginTransmission(8);
  int available = Wire.requestFrom(8, (uint8_t)4);
  
  if(available == 4)
  {
    int receivedValue = Wire.read() << 8 | Wire.read(); 
    int lastbyte = Wire.read() << 8 | Wire.read(); 
    if(receivedValue != 255){
      Serial.print("Id:");
      Serial.print(lastbyte);
      Serial.print(" ");
      Serial.println(receivedValue);
      std::string topic = "/stereoanlage/data/";
      topic += std::to_string(lastbyte);
      char msg_out[20];
      sprintf(msg_out, "%d",receivedValue);
      client.publish(topic.c_str(), msg_out);
    }
  }
  else
  {
    Serial.print("Unexpected number of bytes received: ");
    Serial.println(available);
  }

  int result = Wire.endTransmission();
  if(result)
  {
    Serial.print("Unexpected endTransmission result: ");
    Serial.println(result);
  }
    delay(1000);
}

void setupWifi() {
    delay(10);
    Serial.print("\nVerbinden mit: "); 
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi verbunden mit:");
    Serial.print(WiFi.localIP());
}

void reconnect() {
	while (!client.connected()) {
		Serial.print("Reconnecting...");
		if (!client.connect("Stereoanlage")) {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" retrying in 5 seconds");
			delay(5000);
		}
	}
}