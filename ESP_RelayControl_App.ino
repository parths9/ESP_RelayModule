#include <ESP8266WiFi.h> 
#include <FirebaseESP8266.h>

#define WIFI_SSID "Galaxy M110933"
#define WIFI_PASSWORD "ilsx6565"

#define FIREBASE_HOST "https://espledcontrol-8cd46-default-rtdb.asia-southeast1.firebasedatabase.app/" //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "xQ5mkP97s3oM4yR0evrBoO4gxWCUMoeysGq1RLTl" //Your Firebase Auth Token

FirebaseData fbdo;

const int Relay_pin = 4; //D2
const int Switch = 2; //D4
int Relay_status = 0;
int S2;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  //Firebase.RTDB.setInt(&fbdo, "/RELAY_STATUS", Relay_status);
  pinMode(Relay_pin, OUTPUT); // declare Relay as output 
  pinMode(Switch, INPUT); // declare switch as input
}

void loop() {
//Firebase.RTDB.setInt(&fbdo, "/RELAY_STATUS", Relay_status);
S2 = digitalRead(Switch); // read input value 
Firebase.RTDB.setInt(&fbdo, "/S2", S2);

if(Firebase.RTDB.getInt(&fbdo, "/ESP_RelayControl/S1")) {
 if (fbdo.dataType() == "string") {
 Relay_status = fbdo.stringData().toInt();
 }
}
if (Relay_status == 1 || S2 == HIGH){
  digitalWrite(Relay_pin,LOW); //Device1 is ON
  Serial.println("Relay ON");
  }
  else if (Relay_status == 0 || S2 == LOW){
    digitalWrite(Relay_pin,HIGH); //Device1 if OFF
    Serial.println("Relay OFF");
  }
}