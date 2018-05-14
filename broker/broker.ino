#include <ESP8266WiFi.h>

#include "uMQTTBroker.h"

unsigned int mqttPort = 1883;
unsigned int max_subscriptions = 30;
unsigned int max_retained_topics = 30;

char ssid[] = "yourssid";
char pass[] = "yourpasswd";

void data_callback(uint32_t *client, const char* topic, uint32_t topic_len, const char *data, uint32_t lengh) {
  char topic_str[topic_len+1];
  os_memcpy(topic_str, topic, topic_len);
  topic_str[topic_len] = '\0';

  char data_str[lengh+1];
  os_memcpy(data_str, data, lengh);
  data_str[lengh] = '\0';

  Serial.print("received topic '");
  Serial.print(topic_str);
  Serial.print("' with data '");
  Serial.print(data_str);
  Serial.println("'");
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  MQTT_server_onData(data_callback);

  Serial.println("Starting MQTT broker");
  MQTT_server_start(mqttPort, max_subscriptions, max_retained_topics);
}

void loop()
{
  delay(1000);
}

