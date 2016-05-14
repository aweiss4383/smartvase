/*
 * Created by Alex Weiss
 * 
 * This project uses an ESP8266 Wifi Arduino board
 * 
 */

#include <ESP8266WiFi.h>

const char* ssid = "FBI Security Van2.4";
const char* password = "cantilever";

const char* host = "192.168.1.137";

int sensorPin = A0;
int sensorValue;
int adjustedValue;
int readingsPerDay = 20;

long timeGapHours        = 24 / readingsPerDay;    //hours
long timeGapMinutes      = timeGapHours * 60;      //minutes
long timeGapSeconds      = timeGapMinutes * 60;    //seconds
long timeGapMilliseconds = timeGapSeconds * 1000;  //millisenconds

unsigned long previousTime = 0;

String dataBuffer;

WiFiClient client;
const int httpPort = 80;

void setup() {
  Serial.begin(115200);
  delay(500);
  dataBuffer = "";
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime -  previousTime >= timeGapMilliseconds) {
    previousTime = currentTime;
   
    WiFiConnect();
    readMoisture();
    hostConnect();
    WiFiDisconnect();
    //  Serial.println(timeGapMilliseconds);
    //  delay(timeGapMilliseconds);
  }
}

void WiFiConnect() {
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
}

void readMoisture() {
  sensorValue = analogRead(sensorPin);
  adjustedValue = map(sensorValue, 0, 1024, 0, 100);
}

void hostConnect() {
  int connectCounter = 0;
  //client.connect(host, httpPort);
  Serial.println("Connecting...");
  while (!client.connect(host, httpPort) && connectCounter <= 5) {
    Serial.println("connection failed");
    Serial.println("Attempting to reconnect...");
    connectCounter = connectCounter + 1;
    delay(1500);
  }
  if (client.connect(host, httpPort)) { // REPLACE WITH YOUR SERVER ADDRESS
    Serial.println("Connection established.");
    Serial.println("Sending data...");
    client.print("GET /add.php?");
    client.print("hum1=");
    client.print(adjustedValue);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.1.137"); // SERVER ADDRESS HERE TOO
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Connection: close");
    client.println();
    client.println();
    Serial.println("Data sent.");
  }
  delay(1000);
}

void WiFiDisconnect() {
  if (client.connected()) {
    client.stop();
    Serial.println("Connection severed.");
  }
}


