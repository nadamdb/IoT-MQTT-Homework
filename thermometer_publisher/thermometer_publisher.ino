#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <ESP8266WiFi.h>
#include <MQTT.h>

#define CLIENT_ID "client1"

MQTT myMqtt(CLIENT_ID, "192.168.2.2", 1883);

char ssid[] = "yourssid";
char pass[] = "yourpasswd";

#define DHTPIN            2         
#define DHTTYPE           DHT11     

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void myDataCb(String& topic, String& data);
void myPublishedCb();
void myDisconnectedCb();
void myConnectedCb();

bool mqttConnected=false;

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");

  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");

  delayMS = sensor.min_delay / 1000;

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

  Serial.println("Connecting to MQTT server");  

  myMqtt.onConnected(myConnectedCb);
  myMqtt.onDisconnected(myDisconnectedCb);
  myMqtt.onPublished(myPublishedCb);
  myMqtt.onData(myDataCb);
  
  Serial.println("connect mqtt...");
  myMqtt.connect();
  while(mqttConnected==false){
    delay(200);
    Serial.print(".");
    }
  /*Serial.println("subscribe to topic...");
  myMqtt.subscribe("/IoT-hw/#");*/
  delay(10);
}

//
void loop() {

  int value = 0;

  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    value = event.temperature;
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }

  String topic("/IoT-hw");
  topic += "/temperature";
  
  String valueStr(value);
  Serial.println(valueStr);
  Serial.println(topic);

  // publish value to topic
  boolean result = myMqtt.publish(topic, valueStr);
  Serial.println("   *  message sent");
    
  delay(1000);
}

void myConnectedCb()
{
  Serial.println("connected to MQTT server");
  mqttConnected=true;
}

void myDisconnectedCb()
{
  Serial.println("disconnected. try to reconnect...");
  delay(500);
  myMqtt.connect();
}

void myPublishedCb()
{
  Serial.println("published.");
}

void myDataCb(String& topic, String& data)
{
  
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(data);
}
