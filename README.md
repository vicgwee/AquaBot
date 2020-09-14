# Simple Aquarium Telegram Bot
 An ESP8266 Telegram Bot for lights control of your aquarium

## Features
- Uses WifiManager to configure WiFi Connection and Botfather Token
- Timed/Manual Control of LED lights
- Improved performance by handling multiple commands in one batch, sending single-line messages when possible, and reduced use of String objects

## Hardware
- ESP8266 board (I use the Wemos D1 R2)
- LED strip (I use a 12V White LED Strip)
    - Drive these from the ESP8266 using logic-level MOSFETs like IRLZ44N
    - Pins are defined in lights.h -> light_pins[]
- 12V power supply

## Usage 

### Setup
1. Use PlatformIO to build and upload the code to your ESP8266
2. Create your Telegram bot at https://t.me/botfather
2. Connect to the ESP8266's hotspot and key in your WiFi details, as well as the API Token from the Botfather
3. Wait for it to reboot and enjoy your IoT-enabled aquarium!

### Commands
Commands are defined in commands.h. Each string in commands[]' corresponds to the message that users will send to the bot. This will call a function (starting with cmd_ ). Any text output is copied into replies[], an array of char buffers. 

- Lights
    - /state : Prints the current brightness and timer state
    - /toggle_lights : Toggles whether lights are on or off
    - /toggle_timer : Toggles between timer control and manual control
    - /increase, /decrease : Increases or decreases brightness by adjust_brightness_interval

- Misc
    - /stats : Prints usage and uptime statistics 
    - /geek_stats: Prints other random information about the board

## Customisation

### Timer Profiles 
When timer control is enabled, brightness setting is set by linear scaling between timer states

Implementation in lights.cpp
- timer_states is the # of states that you want to define
- timer_start holds the time (in minutes) after midnight for each state
- timer_chars holds the name of each state (which is printed when /state is called)

### Adding functionality
Simply call your function in loop() of main.cpp. Calling yield() often helps ensure stability on ESP8266, so ensure your functions are called at appropriate time intervals.

## Future Extensions
If I had more time, I'd want to add an automatic fish feeder and a DS18B20 temperature sensor. 

However, due to lack of time, I probably won't be adding more features or maintaining this repo. Feel free to fork it and customise your own IoT-enabled aquarium! :) 
"# simple-aquarium-telegram-bot" 
