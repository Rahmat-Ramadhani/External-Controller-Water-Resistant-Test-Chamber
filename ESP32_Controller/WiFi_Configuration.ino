// Replace with your network credentials
const char* ssidSTA = "Gasoline";
const char* passwordSTA = "gasoline";

/* Put your SSID & Password */
const char* ssidAP = "Water Resistant Test";  // Enter SSID here
const char* passwordAP = "molina123";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);



// Initialize WiFi
void initWiFiSTA() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidSTA, passwordSTA);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}



void initWiFiAP() {
  WiFi.softAP(ssidAP, passwordAP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  
}
