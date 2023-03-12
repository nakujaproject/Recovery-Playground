#include <WiFi.h>
#include "transmitwifi.h"

void mqttCallback(char *topic, byte *message, unsigned int length)
{
    debug("Message arrived on topic:");
    debug(topic);
    debug("Message");
    String messageTemp;

    for (int i = 0; i < length; i++)
    {
        debug((char)message[i]);
    }
    debugln();
    if ((char)message[0] == '0')
    {
        digitalWrite(EJECTION_PIN, LOW);
        client.publish("Control/Ejection/Success", "EJECTION-FAILED");
    }
    else if ((char)message[0] == '1')
    {
        digitalWrite(EJECTION_PIN, HIGH);
        client.publish("Control/Ejection/Success", "EJECTED-SUCCESSFULLY");
    }
}
void create_Accesspoint()
{
    debugln();
    debug("creating access point ");
    debugln("ssid: ");
    debugln(ssid);
    debugln("password: ");
    debugln(password);
    WiFi.softAP(ssid, password);
    debugln("Access point successully created ");
    debugln("IP address: ");
    debugln(WiFi.softAPIP());
    client.setBufferSize(MQTT_BUFFER_SIZE);
    client.setServer(mqtt_server, MQQT_PORT);
    client.setCallback(mqttCallback);
}

void setup_wifi()
{
    debugln();
    debug("Connecting to:");
    debugln(ssid);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
        ;

    {
        delay(500);
        debug(".");
    }
    randomSeed(micros());
    debugln("");
    debugln("WiFi connected");
    debugln("IP address: ");
    debugln(WiFi.localIP());
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.begin(115200);
        debug("Attempting MQTT connection...");
        String clientId = "FCClient-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str()))
        {
            debugln("Connected");
            client.subscribe("Control/Ejection");
        }
        else
        {
            debug("failed,rc=");
            debug(client.state());
            debugln("try again in 5 seconds");
            delay(5000);
        }
    }
}

void sendTelemetryWiFi(Data sv)
{

    char mqttMessage[300];
    sprintf(mqttMessage, "%lld,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.8f\n", sv.timeStamp, sv.altitude, sv.ax, sv.ay, sv.az, sv.gx, sv.gy, sv.gz, sv.filtered_s, sv.filtered_v, sv.filtered_a);

    client.publish("ESP32/Connect/Success", "Timestamp,Altitude,ax,ay,az,gx,gy,gz,filtered_s,filtered_v,filtered_a\n");
    client.publish("ESP32/Connect/Success", mqttMessage);

    debugln("Timestamp, Altitude,ax,ay,az,gx,gy,gz,filtered_s,filtered_v,filtered_a\n");
    debugln(mqttMessage);
}

void handleWiFi(Data sv)
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    sendTelemetryWiFi(sv);
}
