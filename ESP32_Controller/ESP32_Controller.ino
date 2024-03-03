#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include "uRTCLib.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
//#include "SD_Card.h"
//#include "DataBase.h"
//#include "WiFi_Configuration.h"
//#include "WebSocket.h"
//#include "RTC.h"

const byte relayIPX5 = 16;
const byte relayIPX6 = 17;
const byte relayPump = 33;
const byte relayTable = 25;
const byte relayTakeOver = 26; 
const byte buzzer = 27;



// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initSDCard();
  initDataBase(); 
//  initWiFiSTA(); // If you want the controller becom station mode
  initWiFiAP(); // If you want the controller become access point mode
  initSPIFFS();
  initWebSocket();
  URTCLIB_WIRE.begin();
  
  initServer();
  

  pinMode(relayIPX5,OUTPUT);
  pinMode(relayIPX6,OUTPUT);
  pinMode(relayPump,OUTPUT);
  pinMode(relayTable,OUTPUT);
  pinMode(relayTakeOver,OUTPUT);
  pinMode(buzzer,OUTPUT);

  
 
  
}
//
//int i = 0;
//unsigned long testTime = 0;
//unsigned long previousTime = 0;
//unsigned long currentTime = 0;
//unsigned long elapsedTime = 0;
//unsigned long pauseElapsedTime = 0;
//unsigned long totalPauseElapsedTime = 0;
//unsigned long pausePreviousTime = 0;
//unsigned long countdownTime = 0;
//const char* currentIdTest = "";
//const char* typeTest = "";
//const char* ipx5Status = "";
//const char* ipx6Status = "";
//bool runningStatus = false;
//bool initPause = true;

void loop() {
  // put your main code here, to run repeatedly:
  mainProgram();
  sendCurrentTime();
  cleanupClient();
}

void informationTone(int code){
  bool state = true;
  int longTone = code / 10;
  int shortTone = code % 10;

  for(int i = 0; i < longTone; i++){
    digitalWrite(buzzer, HIGH);
    delay(1300);
    digitalWrite(buzzer, LOW);
    delay(300);
  }

  for(int j = 0; j < shortTone; j++){
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    delay(300);
  }
  
}
