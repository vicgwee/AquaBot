#pragma once
#include <Arduino.h>
#include <UniversalTelegramBot.h>

extern UniversalTelegramBot *bot;
const uint8_t BOT_TOKEN_LENGTH = 64;
extern char botToken[BOT_TOKEN_LENGTH];

const uint16_t BOT_INTERVAL = 300; 
extern uint32_t bot_lasttime; 
extern uint32_t messages_replied;

void initBot();
void initOldBotToken();
void checkMessages();
void sendResetMsg();