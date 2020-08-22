#include "../comms/commands.h"
#include "../comms/telegramBot.h"
#include "../lights/lights.h"
#include "../sensor/sensor.h"
#include "../utils/time.h"

const char* commands[] = {
  "/current",
  "/average",
  "/stats",
  "/geek_stats",
  "/toggle_lights",
  "/adjust_brightness", 
  "/increase", 
  "/decrease", 
  "/toggle_timer",
  "/state"
};
const uint8_t n_commands = sizeof(commands)/ sizeof(commands[0]);

char reply1[MAX_REPLY_LENGTH];
char reply2[MAX_REPLY_LENGTH];
char reply3[MAX_REPLY_LENGTH];
char* replies[] = {reply1, reply2, reply3};

int8_t parseCommand(const char *message){
  if (message[0] == '/'){
    if (strlen(message) == 10 && strcmp_P(message, (PGM_P)F("/rgb")) > 0){
      return 127;
    }

    for(int i = n_commands-1; i >= 0; i--){
      if (strcmp(message, commands[i])==0){
        return i;
      }
    }
  }
  return -1;
}

void cmd_current(int i){
  snprintf_P(
    replies[i], MAX_REPLY_LENGTH,
    (PGM_P)F("T: %.1fC     RH: %.1f%%"),
    temps[array_index], hums[array_index]
  );
}

void cmd_average(int i){
  snprintf_P(
    replies[i], MAX_REPLY_LENGTH,
    (PGM_P)F("T: %.1fC     RH: %.1f%%"), 
    tempsum/ARRAY_SIZE, humsum/ARRAY_SIZE
  );
}

void cmd_stats(int i){
  uint32_t time= timeClient.getEpochTime() - start_epoch_time;
  uint32_t day = time / (24 * 3600);
  time = time % (24 * 3600);
  uint8_t hour = time / 3600;
  time %= 3600;
  uint8_t min = time / 60;
  time %= 60;
  uint8_t sec = time;

  snprintf_P(
    replies[i], MAX_REPLY_LENGTH,
    (PGM_P)F("I've been alive for: %lud %uh %um %us\nI've replied %lu times"),
    day,hour,min,sec,messages_replied+1
  );
}

void cmd_geekstats(int i){
  snprintf_P(
    replies[i], MAX_REPLY_LENGTH,
    (PGM_P)F("Last reset reason: %s\nFree Heap: %luB\nFragmentation: %u%%"),
    ESP.getResetReason().c_str(),ESP.getFreeHeap(), ESP.getHeapFragmentation()
  );
}

void cmd_toggleLights(int i){
  if(toggleLights()){
    strcpy_P(replies[i],(PGM_P)F("The lights are on! Fishy hide!"));
  }
  else{
    strcpy_P(replies[i],(PGM_P)F("It's dark, fishy can't see :("));
  }
}

void cmd_adjustBrightness(int message_index){
  return;
}

void cmd_increaseBrightness(int i){
  adjustBrightness(1);
  snprintf_P(
    replies[i], MAX_REPLY_LENGTH,
    (PGM_P)F("With %u%% brightness, you can see fishy and fishy can see you!"),
    brightness/10
  );
}

void cmd_decreaseBrightness(int i){
  adjustBrightness(0);
  snprintf_P(
    replies[i], MAX_REPLY_LENGTH,
    (PGM_P)F("The lights are dimmed to %u%% and fishy likes it!"),
    brightness/10
  );
}

void cmd_toggleTimer(int i){
  if(toggleTimer()){
    strcpy_P(replies[i],(PGM_P)F("Fishy will enjoy a regular day-night cycle"));
  }
  else{
    strcpy_P(replies[i],(PGM_P)F("I'll follow your commands"));
  }
}

void cmd_timeOfDay(int i){
  snprintf_P(
    replies[i], MAX_REPLY_LENGTH,
    (PGM_P)F("It's now %s and the lights are at %u%%"),
    timer_chars[timer_state], brightness/10*lights_on
  );
}

void cmd_unknown(int i){
  strcpy_P(replies[i],  (PGM_P)F("fishy no understand u :("));
}

void cmd_setRGB(const char *message, int i){
  uint32_t num = (uint32_t)strtol(&message[4], NULL, 16);
  setRGBW(num);
  snprintf_P(
    replies[i], MAX_REPLY_LENGTH,
    (PGM_P)F("RGB colour set to (%u,%u,%u)"),
    (uint8_t)rgbw[0], (uint8_t)rgbw[1], (uint8_t)rgbw[2]
  );
}

void messageHandler(const char *message, int i){
  switch(parseCommand(message)){
    case -1:     cmd_unknown(i);
      break;
    case 0:      cmd_current(i);
      break;
    case 1:      cmd_average(i);
      break;
    case 2:      cmd_stats(i);
      break;
    case 3:      cmd_geekstats(i);
      break;
    case 4:      cmd_toggleLights(i);
      break;
    case 5:      cmd_adjustBrightness(i);
      break;
    case 6:      cmd_increaseBrightness(i);
      break;
    case 7:      cmd_decreaseBrightness(i);
      break;
    case 8:      cmd_toggleTimer(i);
      break;
    case 9:      cmd_timeOfDay(i);
      break;
    case 127:    cmd_setRGB(message, i);
      break;
  }
}
