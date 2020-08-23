#include "lights.h"
#include "../utils/time.h"

/*Timer */
const int timer_states = 8;

//RGBW profiles
const int SUNRISE_rgbw[] PROGMEM =   {255,80,0,0};
const int DAY_rgbw[] PROGMEM =       {255,120,112,255};
const int SUNSET_rgbw[] PROGMEM =    {255,80,10,100};
const int TWILIGHT_rgbw[] PROGMEM =  {70,20,255,0};
const int NIGHT_rgbw[] PROGMEM =     {0,0,0,0};

//Timer profiles
const int* timer_rgbws[timer_states] = {
  SUNRISE_rgbw,
  DAY_rgbw,
  DAY_rgbw,
  DAY_rgbw,
  SUNSET_rgbw,
  TWILIGHT_rgbw,
  TWILIGHT_rgbw,
  NIGHT_rgbw
};
const int timer_brightness[timer_states] PROGMEM = {10,700,1000,900,900,250,100,0};

const int timer_start[timer_states] PROGMEM = {
  7 *60 + 30,
  9 *60 + 30,
  12*60 + 30,
  17*60     ,
  18*60 + 30,
  19*60 + 30,
  23*60,
  23*60 + 15
  }; //SR start, SS start, TW start night

const char* timer_chars[timer_states] = {"Sunrise", "Morning", "Afternoon","Sunset", "Evening","Twilight","Night", "Night"};

bool timer_on = 1; 
uint8_t timer_state = 0;

//Lights
const int RGB_RES = 255;
const int MAX_BRIGHTNESS = 1000;

//LED hardware
const int rgbw_pins[] = {D7,D8,D6,D5};
const int PWM_FREQ = 781;
const int PWM_RES = 1023;

uint32_t lights_lasttime;
const int adjust_brightness_interval = 200;

float rgbw[] = {255,160,185, 0}; //stored as float for better resolution
bool lights_on = 0;
int brightness = 0;

void initLights(){
  analogWriteFreq(PWM_FREQ);
  for(int i = 0; i < 4; i++){
    pinMode(rgbw_pins[i], OUTPUT);
    digitalWrite(rgbw_pins[i],0);
  }
}

void writeLights(){
  for(int i = 0; i<4; i++){
    int pwm_val = (rgbw[i]*brightness*lights_on*PWM_RES)/(RGB_RES*MAX_BRIGHTNESS);
    if (pwm_val <= 1 ){
      digitalWrite(rgbw_pins[i],0);
    }
    else if (pwm_val >= PWM_RES - 10 ){
      digitalWrite(rgbw_pins[i],1);
    }
    else{
      analogWrite(rgbw_pins[i],pwm_val);
    }
  }
}

bool toggleLights(){
  lights_on = !lights_on;
  writeLights();
  return (lights_on);
}

bool toggleTimer(){
  timer_on = !timer_on;
  updateLights();
  return (timer_on);
}

void adjustBrightness(bool dir){
  brightness += (-1)*(!dir)*adjust_brightness_interval + dir*adjust_brightness_interval ;
  brightness = max(0, min(MAX_BRIGHTNESS,brightness));
  if (brightness == 0){
    lights_on = 0;
  }
  else{
    lights_on = 1;
  }
  writeLights();
}

float updateTimeOfDay(int mins){
  if (mins < timer_start[0] || mins >= timer_start[timer_states-1]){
    timer_state = timer_states - 1;
    return 0;
  }
  for(int i = 0; i < timer_states-1 ; i++){
    if (mins >= timer_start[i] && mins < timer_start[i+1]){
      timer_state = i;
      return float(mins- timer_start[i])/(timer_start[i+1]-timer_start[i]);
    }
  }
  timer_state = timer_states - 1;
  return 0;
}

void setRGBW(uint32_t hex){
  for(int i = 0; i < 3; i++){
    uint8_t val = (hex >> (16-i*8)) & 0x0000FF;
    rgbw[i] = float(val);
  }
  rgbw[3] = min(rgbw[0],min(rgbw[1],rgbw[2]));
  writeLights();
}

void setTimedLights(float factor){
    Serial.print(F("Timer mode: "));
    Serial.println(timer_chars[timer_state]);
    lights_on = 1;
    if (timer_state == timer_states - 1){ //Night
      if (brightness > 10){
        brightness -= 10;
      }
      else{
        brightness = 0; 
        lights_on = 0; 
      }
      return;
    }

    brightness = timer_brightness[timer_state] * (1-factor) + timer_brightness[timer_state+1] * factor;
  
    for(int i = 0; i < 4; i++){
      rgbw[i] = timer_rgbws[timer_state][i]*(1-factor) + timer_rgbws[timer_state+1][i]*factor;
    }
}

void updateLights(){
  timeClient.update();
  int mins = timeClient.getHours() * 60 + timeClient.getMinutes();

  float factor = updateTimeOfDay(mins);
  if(timer_on){
    setTimedLights(factor);
    writeLights();
  }
}