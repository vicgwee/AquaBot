#include "wifi.h"
#include "../comms/telegramBot.h"

bool shouldSaveConfig;
WiFiManagerParameter custom_bot_id("botid", "Bot Token", botToken, 50);

void saveConfigCallback () {
  Serial.println(F("Should save config"));
  shouldSaveConfig = true;
}

void initWifi(){
  WiFiManager wifiManager;
  Serial.println(F("Send r to reset settings..."));
  delay(2000);
  if (Serial.available()>0){
    if(Serial.read() == 'r'){
      Serial.println(F("Resetting settings"));
      wifiManager.resetSettings(); //RESET SETTINGS
    }
  }
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.addParameter(&custom_bot_id);

  //If it fails to connect it will create a TELEGRAM-BOT access point
  wifiManager.autoConnect("TELEGRAM-BOT");

  Serial.println(F("WiFi connected"));
  Serial.print(F("IP address: "));
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}