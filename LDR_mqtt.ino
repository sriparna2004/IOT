#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "NETGEAR80";
const char* password = "greenballoon707";
/*const char* mqtt_server = "192.168.0.33";*/
const char* mqtt_server = "192.168.0.38";
const int ldrPin = A0;
// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;
int sensorValue = 0;  // variable to store the value coming from the sensor


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char msg1[150];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();
  Serial.println("messageTemp-"+messageTemp);

/*  if(topic=="room/lamp"){
      Serial.println("Changing Room lamp to ");
      if(messageTemp == "on"){
        digitalWrite(lamp, LOW);
        Serial.println("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(lamp, HIGH);
        Serial.println("Off");
      }
  }*/

   
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
   //   client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("room/ldr");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
//  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(ldrPin, INPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  sensorValue = analogRead(ldrPin);
  Serial.println(sensorValue);

  String ldrv=String(sensorValue);
  ldrv.toCharArray(msg, ldrv.length()+1);
//  snprintf (msg, 75, msg1, value);
  client.publish("room/ldr", msg);
  
   
//  Serial.println("sending ON");
//  client.publish("room/lamp", "on");
  delay(3000);
 // Serial.println("sending OFF");
//  client.publish("room/lamp", "off");
  delay(3000);

/*  if (now - lastMeasure > 30000) {
    lastMeasure = now;
    Serial.println("sending ON");
    client.publish("room/lamp", "on");
        
  }
  Serial.println("sending OFF");
  client.publish("room/lamp", "off");*/
  
}
