## LuaArduino

This is a library for embedding a Lua interpreter in Arduino projects.

Works on platforms with large memory footprints, currently requires about 80k of program flash.

## Usage
Example sketch found in examples/src, can be built with Platformio.

##Status
####Builds and runs
   * teensy36
   * teensy31 (Teensy 3.1, 3.2)
   * adafruit_itsybitsy_m4
   * trinket_m0
   * due
   * nano_33_iot

####Builds, is untested
   * teensy35
   * adafruit_feather_nrf52840
   * disco_l475vg_iot01a
   * sipeed-maix-bit

####Builds, does not run:
   * ESP32 lolin32
   * ESP8266 d1_mini

####Does not build:
  * pinoccio

## Origin
Based on initial the work of [AnthonyDiGirolamo](https://github.com/AnthonyDiGirolamo) from [AnthonyDiGirolamo/lua-teensyduino](https://github.com/AnthonyDiGirolamo/lua-teensyduino):

This is Lua 5.1.5 with the following patches:

1. [Lua Tiny RAM (LTR)](http://lua-users.org/lists/lua-l/2008-11/msg00331.html)

   This is used in eLua and nodemcu

1. [Lua Compact Debug (LCD)](https://nodemcu.readthedocs.io/en/master/en/lcd/)

   Original patch can be found here: https://github.com/TerryE/lua-5.1/commits/master

## TODO
 * Fix stdio
 * Add helper methods for adding C callbacks
 * Add Lua bindings for standard Arduino functions

## Helpful Lua C API Links

- (Programming in Lua book)[https://www.lua.org/pil/contents.html]
- (Sample Lua and C code)[https://github.com/tylerneylon/APIsWithLua]
