#pragma once
#include <Arduino.h>

#define MAX_REPLY_LENGTH 128
extern char* replies[];

void messageHandler(const char *message, int i);