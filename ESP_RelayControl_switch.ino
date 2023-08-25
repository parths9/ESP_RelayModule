//#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Galaxy M110933"
#define WIFI_PASSWORD "ilsx6565"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBJEpwv2LieZvMPvX7UW2QA02sOmdfDVnM"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://espledcontrol-8cd46-default-rtdb.asia-southeast1.firebasedatabase.app/" 


//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int Relay_pin = 12; // choose pin for the LED
const int Switch = 13; // choose input pin (for Infrared sensor) 
int Relay_state = 0; // variable for reading the pin status
//String Sign;

bool signupOK = false;

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

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  //pinMode(IR_sensor, INPUT); // declare Infrared sensor as input
  pinMode(Relay_pin, OUTPUT); // declare LED as output 
  pinMode(Switch, INPUT); // declare switch as input
}
void loop()
{
  Relay_state = digitalRead(Switch); // read input value 
   if (Relay_state == HIGH)
   { // check if the input is HIGH
      digitalWrite(Relay_pin, LOW); // turn Relay ON
      Serial.println("Relay ON");   
   } 
   else if (Relay_state == LOW)
   { 
      digitalWrite(Relay_pin, HIGH); // turn Relay OFF
      Serial.println("Relay OFF"); 
   } 
  if (Firebase.ready() && signupOK){
    // Pass the switch value on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "/RELAY_STATE", Relay_state)){
      Serial.println("PASSED");
      //Serial.println("PATH: " + fbdo.dataPath());
      //Serial.println("TYPE: " + fbdo.dataType());
    }
    else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}
