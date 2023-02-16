#include <WiFi.h>
#include "transmitwifi.h"


// FILE *fpt
// fpt = fopen("MyFile.csv","w+");
// char msg[];

int led = 2;
int button = 23;
int buttonState;


void manualOverride(){
  buttonState=digitalRead(button);
  if (buttonState== LOW){
    digitalWrite(led, HIGH);
    client.publish("ESP32/Connect/Success", "EJECTION SIGNAL SENT!");
  }
  else if(buttonState==HIGH){
    digitalWrite(led, LOW);
    client.publish("ESP32/Connect/Success", "EJECTION SIGNAL NOT SENT!");
  }

}

void mqttCallback(char *topic,byte *message, unsigned int length){
    debug("Message arrived on topic:");
    debug(topic);
    debug("Message");
    String messageTemp;

    for (int i=0;i<length;i++){
        debug((char)message[i]);
    }
    debugln();

    if((char)message[0] =='0'){
        digitalWrite(led,HIGH);
        client.publish(topic,"on");
    }
    else if((char)message[0]=='1')
    digitalWrite(led,LOW);
    client.publish(topic,"off");

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
IPAddress IP = WiFi.softAPIP();
debugln("Access point successully created ");
debugln("IP address: ");
debugln(IP);
client.setBufferSize(MQTT_BUFFER_SIZE);
client.setServer(mqtt_server, MQQT_PORT);
client.setCallback(mqttCallback);

}

void setup_wifi(){
    debugln();
    debug("Connecting to:");
    debugln(ssid);
    Serial.begin(115200);
    WiFi.begin(ssid,password);
    // WiFi.RSSI();
    while(WiFi.status() != WL_CONNECTED);

    {
        delay(500);
        debug(".");
    }
    randomSeed(micros());
    debugln("");
    debugln("WiFi connected");
    debugln("IP address: ");
    debugln(WiFi.localIP());

    // client.setBufferSize(MQTT_BUFFER_SIZE);
    // client.setServer(mqtt_server, MQQT_PORT);
    // client.setCallback(mqttCallback);

}

void reconnect(){
    while (!client.connected())
    {
        Serial.begin(115200);
        debugln("Attempting MQTT connection...");
        String clientId = "FCClient-";
        // clientId += String(random(0xffff), HEX);
        if(client.connect(clientId.c_str())){
            debugln("Connected");
            // client.publish((const char*)"controls/ejection",message);
            // Subscribe
             client.subscribe("controls/ejection"); 
            //  client.publish("control/ejection","123");
             client.subscribe("ESP32/Connect/Success")  ;     
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

void sendTelemetryWiFi(Data sv){

    // FILE *mqttMessage;
    
    // mqttMessage = fopen("mqttMessage.csv","w+");

    // fprintf(mqttMessage,"timestamp,altitude,ax,ay,az,gx,gy,gz,filtered_s,filtered_v,filtered_a,longitude,latitude\n");

    // fprintf(mqttMessage,"%lld,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.8f,%.8f\n",sv.timeStamp,sv.altitude,sv.ax,sv.ay,sv.az,sv.gx,sv.gy,sv.gz,sv.filtered_s,sv.filtered_v,sv.filtered_a);
    // client.subscribe("ESP32/Connect/Success");
    // // client.publish("ESP32/Connect/Success", mqttMessage);

    // fclose(mqttMessage);
    char mqttMessage[300];
    sprintf(mqttMessage, "{\"timestamp\":%lld,\"altitude\":%.3f,\"ax\":%.3f,\"ay\":%.3f,\"az\":%.3f,\"gx\":%.3f,\"gy\":%.3f,\"gz\":%.3f,\"filtered_s\":%.3f,\"filtered_v\":%.3f,\"filtered_a\":%.3f,\"longitude\":%.3f,\"latitude\":%.3f,\"RSSI\":%.3f}", sv.timeStamp, sv.altitude,sv.ax,sv.ay,sv.az,sv.gx,sv.gy,sv.gz,sv.filtered_s,sv.filtered_v,sv.filtered_a,sv.longitude, sv.latitude,sv.RSSI);
    client.subscribe("ESP32/Connect/Success");
    client.publish("ESP32/Connect/Success", mqttMessage);
    debugln(mqttMessage);
    delay(500);

}


void handleWiFi(Data sv){
    if(!client.connected()){
        reconnect();
    }
    client.loop();
    manualOverride();
    sendTelemetryWiFi(sv);
}

