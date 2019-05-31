## LuaArduino

This is a library for embedding a Lua interpreter in Arduino projects.

Tested on Arduino platforms with large memory footprints, including Teensy 3.6 & 3.2 and Adafruit ItsyBitsy M4, currently requires about 80k of program flash.
Does not work on ESP32, yet.

Example sketch found in examples/src, can be built with Platformio.

Based on initial the work of [AnthonyDiGirolamo](https://github.com/AnthonyDiGirolamo) from [AnthonyDiGirolamo/lua-teensyduino](https://github.com/AnthonyDiGirolamo/lua-teensyduino):

This is Lua 5.1.5 with the following patches:

1. [Lua Tiny RAM (LTR)](http://lua-users.org/lists/lua-l/2008-11/msg00331.html)

   This is used in eLua and nodemcu

1. [Lua Compact Debug (LCD)](https://nodemcu.readthedocs.io/en/master/en/lcd/)

   Original patch can be found here: https://github.com/TerryE/lua-5.1/commits/master

## Helpful Lua C API Links

- https://www.lua.org/pil/contents.html
- https://github.com/tylerneylon/APIsWithLua
