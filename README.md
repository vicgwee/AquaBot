# Aquarium Telegram Bot
 An ESP8266 Telegram Bot for lights control and temperature monitoring of your aquarium

## Features
- Uses WifiManager to configure WiFi Connection and Botfather Token
- Timed/Manual Control of RGB + W LED lights
- Read SHT30 Temperature and Humidity Sensor on the I2C bus

## Hardware
- ESP8266 board (I use the Wemos D1 R2)
- SHT30D Sensor
- RGBW LED strip (I use a separate RGB and W LED Strip)
    - Drive these from the ESP8266 using logic-level MOSFETs like IRLZ44N
    - Pins are defined in lights.h -> rgbw_pins[] , change as needed
- Well-sized 5V power supply (I use one buck converter to power each of the RGB strips)

## Setup 
1. Use PlatformIO to build and upload the code to your ESP8266
2. Create your Telegram bot at https://t.me/botfather
2. Connect to the ESP8266's hotspot and key in your WiFi details, as well as the API Token from the Botfather
3. Wait for it to reboot and enjoy your IoT-enabled aquarium!

## Commands

Commands are defined in commands.h. Feel free to add on what you want!
- Lights
    - /state : Prints the current brightness and timer state
    - /toggle_lights : Toggles whether lights are on or off
    - /toggle_timer : Toggles timer control and manual control
    - /increase, /decrease : Increases or decreases brightness by adjust_brightness_interval
    - /rgbXXXXXX : Sets the RGB value to 0xXXXXXX
- Temperature/Humidity
    - /current : Prints current temperature/humidity
    - /average : Prints averaged temperature/humidity. A reading is taken every SENSOR_INTERVAL and ARRAY_SIZE readings are averaged
- Misc
    - /stats : Prints usage and uptime statistics 
    - /geek_stats: Prints other random information about the board

## Customisation

### RGBW profiles 
- When timer control is enabled, RGBW is controlled via RGBW profiles
- Implementation in lights.cpp
    - Arrays of length 4 ending with _rgbw 
    - Range: 0 to RGB_RES

### Timer Profiles 
- When timer control is enabled, brightness and RGBW setting is set by linear scaling between timer states
- Implementation in lights.cpp
    - timer_states is the # of states that you want to define
    - timer_rgbws points to the RGBW profile
    - timer_start indicates the time (in minutes) after midnight for each state
    - timer_chars indicates the name of each state (printed when /state is called)

I will be adding more documentation, but probably will not be adding more features or maintaining this repo. Feel free to fork it and customise your own IoT-enabled aquarium! :) 
