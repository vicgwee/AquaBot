#pragma once
#include <Arduino.h>

extern const char* timer_chars[];

extern bool lights_on;
extern bool timer_on;
extern uint8_t timer_state;
extern int brightness;

const uint32_t LIGHTS_INTERVAL = 59999;
extern uint32_t lights_lasttime;
extern float rgbw[];

void initLights();

int pwm_scaling(float brightness);

void writeLights();

bool toggleLights();

bool toggleTimer();

void adjustBrightness(bool dir);

void setRGBW(uint32_t hex);

void updateLights();