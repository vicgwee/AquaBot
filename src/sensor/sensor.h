#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <ClosedCube_SHT31D.h>

extern ClosedCube_SHT31D sht3xd;

const uint32_t SENSOR_INTERVAL = 60000;
extern uint32_t sensor_lasttime;
const uint8_t ARRAY_SIZE = 32;
extern uint8_t array_index; 
extern float temps[ARRAY_SIZE];
extern float hums[ARRAY_SIZE];
extern float tempsum;
extern float humsum;

void initSensor();
void updateSensorArray();
