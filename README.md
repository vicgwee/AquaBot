# Aquarium Telegram Bot
 An ESP8266 Telegram Bot for lights control and temperature monitoring of your aquarium

## Features
- Uses WifiManager to configure WiFi Connection and Botfather Token
- Timed/Manual Control of RGB + W LED lights
- Read SHT30 Temperature and Humidity Sensor on the I2C bus
- Improved performance by handling multiple commands in one batch, sending single-line messages when possible, and reduced use of String objects

## Hardware
- ESP8266 board (I use the Wemos D1 R2)
- SHT30D Sensor
- RGBW LED strip (I use separate RGB and White LED Strip)
    - Drive these from the ESP8266 using logic-level MOSFETs like IRLZ44N
    - Pins are defined in lights.h -> rgbw_pins[] , change as needed
- 5V power supply (I use one buck converter to power each of the RGB strips)

## Usage 

### Setup
1. Use PlatformIO to build and upload the code to your ESP8266
2. Create your Telegram bot at https://t.me/botfather
2. Connect to the ESP8266's hotspot and key in your WiFi details, as well as the API Token from the Botfather
3. Wait for it to reboot and enjoy your IoT-enabled aquarium!

### Commands
Commands are defined in commands.h. Each string in  commands[]' corresponds to the message that users will send to the bot. This will call a function (starting with cmd_ ). Any text output is copied into replies[], an array of char buffers. 

- Lights
    - /state : Prints the current brightness and timer state
    - /toggle_lights : Toggles whether lights are on or off
    - /toggle_timer : Toggles between timer control and manual control
    - /increase, /decrease : Increases or decreases brightness by adjust_brightness_interval
    - /rgbXXXXXX : Sets the RGB value to XXXXXX (hex value)
- Temperature/Humidity
    - /current : Prints current temperature/humidity
    - /average : Prints averaged temperature/humidity. A reading is taken every SENSOR_INTERVAL and ARRAY_SIZE readings are averaged
- Misc
    - /stats : Prints usage and uptime statistics 
    - /geek_stats: Prints other random information about the board

## Customisation

### RGBW profiles 
When timer control is enabled, RGBW is controlled via RGBW profiles. Each profile holds 4 integers which represent the relative brightness of red, green, blue and white respectively

Implementation in lights.cpp
- Arrays of ending with _rgbw 
- Range: 0 to RGB_RES

### Timer Profiles 
When timer control is enabled, brightness and RGBW setting is set by linear scaling between timer states

Implementation in lights.cpp
- timer_states is the # of states that you want to define
- timer_rgbws holds pointers to each RGBW profile for each state
- timer_start holds the time (in minutes) after midnight for each state
- timer_chars holds the name of each state (which is printed when /state is called)

### Adding functionality
Simply call your function in loop() of main.cpp. Calling yield() often helps ensure stability on ESP8266, so ensure your functions are called at appropriate time intervals.

## Future Extensions
If I had more time, I'd want to add an automatic fish feeder and a DS18B20 temperature sensor. 

However, due to lack of time, I probably won't be adding more features or maintaining this repo. Feel free to fork it and customise your own IoT-enabled aquarium! :) 
