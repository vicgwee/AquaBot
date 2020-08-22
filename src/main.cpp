#include "comms/commands.h"
#include "comms/telegramBot.h"
#include "sensor/sensor.h"
#include "lights/lights.h"
#include "utils/time.h"
#include "utils/wifi.h"

void setup() {
  initLights();
  Serial.begin(1000000L);
  initSensor();
  initOldBotToken();
  initWifi();
  initBot();
  initTimeClient();
  updateLights();
  sendResetMsg();
}

void loop() {
  if (millis() > bot_lasttime + BOT_INTERVAL)  {
    checkMessages();
    bot_lasttime = millis();
  }
  else if (millis() > sensor_lasttime + SENSOR_INTERVAL){
    updateSensorArray();
    sensor_lasttime += SENSOR_INTERVAL;
  }
  else if (millis() > lights_lasttime + LIGHTS_INTERVAL){
    updateLights();
    lights_lasttime += LIGHTS_INTERVAL;
  }
  yield();
}