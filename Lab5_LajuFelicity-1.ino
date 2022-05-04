#include "DHT.h"
#include "PubSubClient.h"
#include "WiFi.h"

#define DHTPIN 15     

#define DHTTYPE DHT22   

DHT dht(DHTPIN, DHTTYPE);

//WiFi ANDROID DEVICE *****
const char* ssid = "Mindyabusiness";                 // Your personal network SSID
const char* wifi_password = "fall2021"; // Your personal network password

//change MQTT*****
const char* mqtt_server = "192.168.43.209";  // IP of the MQTT broker
const char* humidity_topic = "greenhouse/humidity";
const char* temp_topic = "greenhouse/temperature"; 
const char* ldr_topic = "greenhouse/lightintensity"; 
const char* depth_topic = "greenhouse/depth"; 
const char* mqtt_username = "laju"; // MQTT username***
const char* mqtt_password = "laju"; // MQTT password***
const char* client_id = "farm"; // MQTT client ID

//initialise the WiFi and MQTT 
WiFiClient wifi_client;
PubSubClient client(mqtt_server, 1883, wifi_client); 

const int ldrPin = 34;  // Analog input pin that the ldr is attached to
int RED = 19;
int water = 39;
int sensorValue=0;  
int outputValue=0;



// Custom function to connet to the MQTT broker via WiFi

  void connect_MQTT() {
  
  Serial.print("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Connection message
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

if (client.connect(client_id, mqtt_username, mqtt_password)) {
    Serial.println("MQTT connection successful");
  }
  else {
    Serial.println("MQTT connection failed");
  }
}

void setup() {
  Serial.begin(9600);

  Serial.println(F("DHT test!"));

  dht.begin();
  pinMode(ldrPin,INPUT);


}

void loop()
{
    //Serial.println( mqtt_username);
    //Serial.println( mqtt_password); 
    connect_MQTT();
    Serial.setTimeout(2000);

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.print("Humidity:");
    Serial.println(h);
    Serial.print("Temperature:");
    Serial.println(t);
    sensorValue = analogRead(ldrPin);
    outputValue = map(sensorValue, 0, 4095, 0, 255);
    Serial.print("LDR:");
    Serial.println(outputValue);
    int value = analogRead(39);
    Serial.print("depth:");
    Serial.println(value);


  if (client.publish(temp_topic, String(t).c_str())) {
    Serial.println("Temperature value sent!");
  }
  else {
    Serial.println("Failed! Reconnecting to MQTT Broker...");
    client.connect(client_id, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(temp_topic, String(t).c_str());
  }

 if (client.publish(humidity_topic, String(h).c_str())) {
    Serial.println("Humidity value sent!");
  }
  else {
    Serial.println("Failed! Reconnecting to MQTT Broker...");
    client.connect(client_id, mqtt_username, mqtt_password);
    delay(10); 
    client.publish(humidity_topic, String(h).c_str());
  }
if (client.publish(ldr_topic, String(outputValue).c_str())) {
    Serial.println("Light intensity sent!");
  }
else {
    Serial.println("Light intensity failed to send. Reconnecting to MQTT Broker...");
    client.connect(client_id, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(ldr_topic, String(outputValue).c_str());
  }
  if (client.publish(depth_topic, String(value).c_str())) {
    Serial.println("Waterlevel sent!");
  }
else {
    Serial.println("Waterlevel failed to send. Reconnecting to MQTT Broker...");
    client.connect(client_id, mqtt_username, mqtt_password);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(depth_topic, String(value).c_str());
  client.disconnect();  // disconnect from the MQTT broker
  delay(1000*30);       // print new values every 30s
}
}

Regards,

Felicity Ibukunoluwa Oguntolu
EEE Major
Class of 2022

________________________________________
From: Ofeoritselaju Oyan <ofeoritselaju.oyan@aucampus.onmicrosoft.com>
Sent: Thursday, December 16, 2021 2:21:09 AM
To: Felicity ibukunoluwa Oguntolu <felicity.oguntolu@aucampus.onmicrosoft.com>
Subject: 
 
"""Python script for sending data to db"""
 
import paho.mqtt.client as mqtt
import mysql.connector
 
 
MQTT_ADDRESS = 'localhost'
MQTT_TOPIC = 'IoTClass/+/+'
 
db_conn = mysql.connector.connect(
        host='localhost', database='hydroponics', user='root', password=''
    )
 
temp1 = "IoTClass/devices/temp1"
temp2 = "IoTClass/devices/temp2"
ldr = "IoTClass/devices/ldr"
heater = "IoTClass/devices/heater"
humidity1 = "IoTClass/devices/humidity1"
humidity2 = "IoTClass/devices/humidity2"
pump = "IoTClass/devices/pump"
 
t1 = 0
t2 = 0
l1 = 0
h1 = 0
hum1 = 0
hum2 = 0
p1 = 0
 
 
def on_connect(client, userdata, flags, conn_result):
    client.subscribe(MQTT_TOPIC)
 
 
def on_message(client, userdata, msg):
    print(msg.topic + ' ' + str(msg.payload.decode("utf-8")))
    
    if msg.topic == temp1:
        global t1
        t1 = str(msg.payload.decode("utf-8"))
 
    if msg.topic == temp2:
        global t2
        t2 = str(msg.payload.decode("utf-8"))
 
    if msg.topic == ldr:
        global l1
        l1 = str(msg.payload.decode("utf-8"))
 
    if msg.topic == heater:
        global h1
        h1 = str(msg.payload.decode("utf-8"))
 
    if msg.topic == humidity1:
        global hum1
        hum1 = str(msg.payload.decode("utf-8"))
 
    if msg.topic == humidity2:
        global hum2
        hum2 = str(msg.payload.decode("utf-8"))
     
    if msg.topic == pump:
        global p1
        p1 = str(msg.payload.decode("utf-8"))
 
 
    values = (l1, p1, t1, hum1, hum2, t2, h1)
 
    sql = "INSERT INTO iotsensorvalues (ldr,pump,temp1,humidity1,humidity2,temp2,heater) VALUES ('%s','%s','%s','%s','%s','%s','%s')" % (values)
    cursor = db_conn.cursor()
    cursor.execute(sql)
    db_conn.commit()
    cursor.close()
 
def main():
    sql = """
        CREATE TABLE IF NOT EXISTS iotsensorvalues (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            temp1 DOUBLE NOT NULL,
            temp2 DOUBLE NOT NULL,
            ldr DOUBLE NOT NULL,
            heater VARCHAR(50) NOT NULL,
            humidity1 DOUBLE NOT NULL,
            humidity2 DOUBLE NOT NULL,
            pump VARCHAR(50) NOT NULL
        )
        """
    cursor = db_conn.cursor()
    cursor.execute(sql)
    cursor.close()
 
    mqtt_client = mqtt.Client()
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message
 
    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_forever()
 
 
if _name_ == '_main_':
    print('MQTT to InfluxDB bridge')
    main()
