
/*************************************************************************************/
void setup_wifi() {  
  WiFi.config(ip, gateway, subnet, DNS);
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_key);
  
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
  }
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println();
    Serial.println("Fail connecting");
    delay(5000);
    ESP.restart();
  }
  Serial.println("   OK  ");
  Serial.print("Module IP: ");
  Serial.println(WiFi.localIP());  
}
