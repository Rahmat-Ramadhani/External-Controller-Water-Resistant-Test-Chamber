//#include "DataBase.h"


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

void cleanupClient(){
  ws.cleanupClients();
}


void notifyClients(String dataBase) {
  ws.textAll(dataBase);
//  Serial.print("remaining time : ");
//  Serial.println(myDataBase["running"]["remainingTime"]);
//  
//  Serial.print("last Elapsed Time : ");
//  Serial.println(lastElapsedTime);
}

void sendDataBase(){
  
  String stringMyDataBase = JSON.stringify(myDataBase);
  notifyClients(stringMyDataBase);
  Serial.print("remaining time : ");
  Serial.println(myDataBase["running"]["remainingTime"]);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "getData") == 0) {
      //if it is, send current sensor readings
      
      String stringMyDataBase = JSON.stringify(myDataBase);
      
      notifyClients(stringMyDataBase);
      Serial.print((char*)data);
      
    } else {
      receivedPackage = JSON.parse((char*)data);

      if (JSON.typeof(receivedPackage) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
  
      Serial.print("JSON.typeof(receivedPackage) = ");
      Serial.println(JSON.typeof(receivedPackage));
  
      updateMyDataBase();
      sendDataBase();
      
    }
 

  }
}


void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void initServer() {
  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  // Start server
  server.begin();
}
