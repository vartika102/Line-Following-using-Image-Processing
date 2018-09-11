/*
    This sketch sends a message to a TCP server

*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("ROBOTICS SOCIETY", "vssutrobotix");

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
analogWrite(D0, 512);
analogWrite(D1, 512);
analogWrite(D2, 512);
analogWrite(D3, 512);
}

int mx_spd = 500;

void motion(int spd)
{
  if(spd<=0)
  {
    analogWrite(D0,mx_spd-spd);
    analogWrite(D1,0);
    analogWrite(D2,mx_spd);
    analogWrite(D3,0);
  }
  else
  {
    analogWrite(D0,mx_spd);
    analogWrite(D1,0);
    analogWrite(D2,mx_spd+spd);
    analogWrite(D3,0); 
  }
}


void loop()
{
  const uint16_t port = 1234;
  const char * host = "192.168.1.101"; // ip or dns

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    //delay(5000);
    return;
  }
  int s=0;
  int i=0;
  int c;
  int spd=0;
l1:
  spd=0;
  String k = client.readStringUntil('\r');
  //Serial.println(k);
  spd=k.toInt();
  //Serial.println(spd);
  motion(spd);
  goto l1;
}
