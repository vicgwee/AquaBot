#include "telegramBot.h"
#include "commands.h"
#include "../utils/wifi.h"
#include <WiFiClientSecure.h>
#include <EEPROM.h>

WiFiClientSecure client;
UniversalTelegramBot *bot;

const uint8_t address_offset = 0;
const uint8_t SIMPLE_MESSAGE_LENGTH = 64; 
char botToken[BOT_TOKEN_LENGTH] = "";
uint32_t messages_replied;
uint32_t bot_lasttime;   //last time messages' scan has been done

void initBot(){
  if (shouldSaveConfig) {
    strcpy(botToken, custom_bot_id.getValue());
    Serial.print(F("Saving bot token to EEPROM:\t"));
    Serial.println(botToken);
    EEPROM.put(address_offset, botToken);
    EEPROM.commit();
  }
  EEPROM.end();
  bot = new UniversalTelegramBot(botToken, client);
  client.setInsecure();
}

void initOldBotToken(){
  EEPROM.begin(BOT_TOKEN_LENGTH);
  Serial.println(F("read bot token"));
  EEPROM.get(address_offset, botToken);
  Serial.println(botToken);
}

void replyMessage(int i){ 
    if ((strlen(replies[i]) < SIMPLE_MESSAGE_LENGTH) && (strchr(replies[i],'\n') == NULL) ){
      bot->sendSimpleMessage(bot->messages[i].chat_id,replies[i],"");
    }
    else{
      bot->sendMessage(bot->messages[i].chat_id,replies[i],"");
    }
}

void checkMessages(){
    uint8_t numNewMessages = bot->getUpdates(bot->last_message_received + 1);
    for (int i=0; i<numNewMessages; i++) {
      uint32_t reply_time = millis();
      Serial.print(F("Message: "));
      Serial.print(bot->messages[i].text.c_str());
      Serial.print(F(" from: "));
      Serial.println(bot->messages[i].chat_id.c_str());
      messageHandler(bot->messages[i].text.c_str(), i);
      messages_replied++;
    }
    for (int i=0; i<numNewMessages;i++){
      replyMessage(i);
      Serial.print(F("Replied "));
      Serial.println(bot->messages[i].chat_id.c_str());
    }
}

void sendResetMsg(){
  String msg = "Reset! Last reset reason: " + ESP.getResetReason();
  //bot->sendSimpleMessage("put your telegram ID here",msg,"");
}