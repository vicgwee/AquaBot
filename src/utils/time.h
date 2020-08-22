#pragma once
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

extern NTPClient timeClient;
extern uint32_t start_epoch_time;

void initTimeClient();