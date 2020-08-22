#include "sensor.h"

ClosedCube_SHT31D sht3xd;
#define SENSOR_I2C_ADDRESS 0x45

uint32_t sensor_lasttime;
uint8_t array_index = 0; 

float temps[ARRAY_SIZE];
float hums[ARRAY_SIZE];
float tempsum;
float humsum;

void initSensor(){
  Wire.begin();
  sht3xd.begin(SENSOR_I2C_ADDRESS); 
  Serial.print(F("Serial #"));
  Serial.println(sht3xd.readSerialNumber());
  if (sht3xd.periodicStart(SHT3XD_REPEATABILITY_HIGH, SHT3XD_FREQUENCY_HZ5) != SHT3XD_NO_ERROR)
    Serial.println(F("[ERROR] Cannot start periodic mode"));
    
  SHT31D result = sht3xd.periodicFetchData();
  for(int i = ARRAY_SIZE - 1; i >= 0; i--){
    temps[i] = result.t;
    hums[i] = result.rh;
  }
  tempsum = result.t * ARRAY_SIZE;
  humsum = result.rh * ARRAY_SIZE;
}

void updateSensorArray(){
  SHT31D result = sht3xd.periodicFetchData();
  array_index++;
  if (array_index >= ARRAY_SIZE){
    array_index = 0;
  }
  tempsum -= temps[array_index];
  humsum -= hums[array_index];
  
  temps[array_index] = result.t;
  hums[array_index] = result.rh;

  tempsum += temps[array_index];
  humsum += hums[array_index];
}
