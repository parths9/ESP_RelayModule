#include<Arduino.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

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

const int Relay_pin = 4;
int Relay_status = 0;

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
  
  pinMode(Relay_pin, OUTPUT); // declare Relay as output 
}

void loop(){
  if (Firebase.ready() && signupOK){
    // Pass the Relay_status value on the database path test/int
   if (Firebase.RTDB.setInt(&fbdo, "/RELAY_STATUS", Relay_status)){
    //  Serial.println("PASSED");
    if (Firebase.RTDB.getInt(&fbdo, "/RELAY_STATUS")) {
    if (fbdo.dataType() == "int") {
    // Relay_status = fbdo.stringData();
    Relay_status = fbdo.intData();
    Serial.println(Relay_status);
    if (Relay_status == 1){
              digitalWrite(Relay_pin,LOW); //Device1 is ON
              Serial.println("Relay ON");
          }
          else if (Relay_status == 0){
              digitalWrite(Relay_pin,HIGH); //Device1 if OFF
              Serial.println("Relay OFF");
            }}}
   }
  else 
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }  
}