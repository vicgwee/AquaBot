#include "time.h"

uint32_t start_epoch_time;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "sg.pool.ntp.org", 8*3600, 3600000);

void initTimeClient(){
  timeClient.begin();
  timeClient.update();
  start_epoch_time = timeClient.getEpochTime();
}