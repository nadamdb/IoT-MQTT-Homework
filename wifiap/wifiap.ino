#include <ESP8266WiFi.h>

#include <MQTT.h>

void myDataCb(String& topic, String& data);
void myPublishedCb();
void myDisconnectedCb();
void myConnectedCb();

#define CLIENT_ID "ap"
#define TOPIC "/IoT-hw/temperature"


// create MQTT
MQTT myMqtt(CLIENT_ID, "192.168.2.200", 1883);



char ssid[] = "yourssid";    // your network SSID (name)
char pass[] = "yourpasswd";  // your network password

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
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

  // setup callbacks
  myMqtt.onConnected(myConnectedCb);
  myMqtt.onDisconnected(myDisconnectedCb);
  myMqtt.onPublished(myPublishedCb);
  myMqtt.onData(myDataCb);
  
  Serial.println("connect mqtt...");
  myMqtt.connect();

  Serial.println("subscribe to topic...");
  myMqtt.subscribe(TOPIC);
Serial.println("finishd subsribe");
  delay(10);
}
bool hot = false;
void loop() {
  if (hot) {
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  delay(100);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void myConnectedCb()
{
  Serial.println("connected to MQTT server");
}

void myDisconnectedCb()
{
  Serial.println("disconnected. try to reconnect...");
  delay(500);
  myMqtt.connect();
}

void myPublishedCb()
{
  //Serial.println("published.");
}

void myDataCb(String& topic, String& data)
{
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(data);
  if (atoi(data.c_str()) >= 30 ) hot = true;
  else if (atoi(data.c_str()) != 0) hot = false;
}
