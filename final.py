import paho.mqtt.client as mqtt
import mysql.connector
from mysql.connector import Error
import time

MQTT_SERVER = "127.0.0.1"
MQTT_PATH = "IoTClass/devices/#"

temp1 = 0
temp2 = 0
ldr = 0
heater = ""
humidity1 = 0
humidity2 = 0
pump = ""

#Set up connection to database and insert data
def insertVariablesIntoTable(temp1P, temp2P, ldrP, heaterP, humidity1P, humidity2P, pumpP):

    try:
        connection = mysql.connector.connect(host='127.0.0.1',
                                             database='final_project',
                                             user='root',
                                             password='')
        cursor = connection.cursor()
        mySql_insert_query = """INSERT INTO sensor_data (temp1, temp2, ldr, heater, humidity1, humidity2, pump) 
                                VALUES (%s, %s, %s, %s, %s, %s, %s) """

        recordTuple = (temp1P, temp2P, ldrP, heaterP, humidity1P, humidity2P, pumpP)
        cursor.execute(mySql_insert_query, recordTuple)
        connection.commit()
        print("Record inserted successfully")

    except mysql.connector.Error as error:
        print("Failed to insert into MySQL table {}".format(error))

    finally:
        if (connection.is_connected()):
            cursor.close()
            connection.close()
            print("MySQL connection is closed")


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe(MQTT_PATH)


def on_message(client, userdata, msg):
   # print(msg.topic+" "+str(msg.payload))
    global temp1, temp2, ldr, heater, humidity1, humidity2, pump

    if msg.topic == "IoTClass/devices/temp1":
        temp1 = msg.payload.decode('utf-8')
    elif msg.topic == "IoTClass/devices/temp2":
        temp2 = msg.payload.decode('utf-8')
    elif msg.topic == "IoTClass/devices/ldr":
        ldr = msg.payload.decode('utf-8')
    elif msg.topic == "IoTClass/devices/heater":
        heater = msg.payload.decode('utf-8')
    elif msg.topic == "IoTClass/devices/humidity1":
        humidity1 = msg.payload.decode('utf-8')
    elif msg.topic == "IoTClass/devices/humidity2":
        humidity2 = msg.payload.decode('utf-8')
    elif msg.topic == "IoTClass/devices//pump":
        pump = msg.payload.decode('utf-8')
    else:
        print(msg.payload.decode('utf-8'))

    print(temp1, temp2, ldr, heater, humidity1, humidity2, pump)

    insertVariablesIntoTable(temp1, temp2, ldr, heater, humidity1, humidity2, pump)



client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_SERVER, 1883, 60)
client.loop_forever()

