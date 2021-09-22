#include "FS.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <DHT.h>

#define DHTTYPE DHT11

// Sensor Pin
uint8_t Soil1Pin = D3;
uint8_t Soil2Pin = D4;
uint8_t DHTPin = D5;

//Relay Pin
uint8_t Relay1 = D1;
uint8_t Relay2 = D2;
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

float temperature;
float humidity;
int soil1;
int soil2;

char* kelembapan_tanah_1 = "Lembab"; //Lembab & Kering
char* kelembapan_tanah_2 = "Lembab";
char* status_penyiraman_1 = "Tidak Disiram"; //Sudah Disiram & Tidak Disiram
char* status_penyiraman_2 = "Tidak Disiram";
char* mMessages;

const char* ssid = "Rafeyosa"; //Nama Wifi
const char* password = "kucingimut"; //Password Wifi

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char* AWS_endpoint = "XXXXX-ats.iot.us-east-1.amazonaws.com"; //MQTT broker ip

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

WiFiClientSecure espClient;
PubSubClient client(AWS_endpoint, 8883, callback, espClient); //set MQTT port number to 8883 as per //standard
long lastMsg = 0;
char msg[500];
int value = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  espClient.setBufferSizes(512, 512);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin();
  while(!timeClient.update()){
    timeClient.forceUpdate();
  }

  espClient.setX509Time(timeClient.getEpochTime());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESPthing")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      
      char buf[256];
      espClient.getLastSSLError(buf,256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);
      
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Soil1Pin, INPUT);
  pinMode(Soil2Pin, INPUT);
  pinMode(DHTPin, INPUT);
  dht.begin();
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  setup_wifi();
  delay(1000);
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  
  Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());
  
  // Load certificate file
  File cert = SPIFFS.open("/cert.der", "r"); //replace cert.crt eith your uploaded file name
  if (!cert) {
    Serial.println("Failed to open cert file");
  }
  else
    Serial.println("Success to open cert file");
  
  delay(1000);
  if (espClient.loadCertificate(cert))
    Serial.println("cert loaded");
  else
    Serial.println("cert not loaded");
  
  // Load private key file
  File private_key = SPIFFS.open("/private.der", "r"); //replace private eith your uploaded file name
  if (!private_key) {
    Serial.println("Failed to open private cert file");
  }
  else
    Serial.println("Success to open private cert file");
  
  delay(1000);
  
  if (espClient.loadPrivateKey(private_key))
    Serial.println("private key loaded");
  else
    Serial.println("private key not loaded");
  
  // Load CA file
  File ca = SPIFFS.open("/ca.der", "r"); //replace ca eith your uploaded file name
  if (!ca) {
    Serial.println("Failed to open ca ");
  }
  else
    Serial.println("Success to open ca");
  
  delay(1000);
  
  if(espClient.loadCACert(ca))
    Serial.println("ca loaded");
  else
    Serial.println("ca failed");
  
  Serial.print("Heap: "); Serial.println(ESP.getFreeHeap());
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;

    sensorRead();
    
    snprintf (msg, 500, mMessages, 1, temperature, kelembapan_tanah_1 ,status_penyiraman_1, 2, temperature, kelembapan_tanah_2 ,status_penyiraman_2);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
    Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  }
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(100); // wait for a second;
  digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  delay(100); // wait for a second
}

void sensorRead() {
  temperature = dht.readTemperature(); // membaca data temperature (suhu)
  //humidity = dht.readHumidity(); // membaca data humidity 
  soil1 = digitalRead(Soil1Pin); // 0 == Kering
  soil2 = digitalRead(Soil2Pin);

  if(soil1) {
    kelembapan_tanah_1 = "Lembab";
  } else {
    kelembapan_tanah_1 = "Kering";
  }
  if(soil2) {
    kelembapan_tanah_2 = "Lembab";
  } else {
    kelembapan_tanah_2 = "Kering";
  }
  
  if((temperature>=35)&&(soil1==0)) {
    digitalWrite(Relay1, LOW); // Relay == On
    status_penyiraman_1 = "Sudah Disiram";
  }
  else {
    digitalWrite(Relay1, HIGH); // Relay == Off
    status_penyiraman_1 = "Tidak Disiram";
  }
    
  if((temperature>=35)&&(soil2==0)) {
    digitalWrite(Relay2, LOW); // Relay == On
    status_penyiraman_2 = "Sudah Disiram";
  }
  else {
    digitalWrite(Relay2, HIGH); // Relay == Off
    status_penyiraman_2 = "Tidak Disiram";
  }

  mMessages= "{\"data\":[{\"id\":%d,\"suhu\":\"%.2f\",\"kelembapan_tanah\":\"%s\",\"status_penyiraman\":\"%s\"},{\"id\":%d,\"suhu\":\"%.2f\",\"kelembapan_tanah\":\"%s\",\"status_penyiraman\":\"%s\"}]}";
}
