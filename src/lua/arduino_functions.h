#include <Arduino.h>
#include <LuaArduino.h>
#include <SPI.h>

#ifndef arduino_functions_h
#define arduino_functions_h

static int l_pinMode(lua_State *L) {
    double pin = lua_tonumber(L, 1);
    double mode = lua_tonumber(L, 2);
    pinMode((int)pin, (int)mode);
    return 0;
}

static int l_digitalWrite(lua_State *L) {
    double pin = lua_tonumber(L, 1);
    double state = lua_tonumber(L, 2);
    digitalWrite((int)pin, (int)state);
    return 0;
}

static int l_digitalRead(lua_State *L) {
    double pin = lua_tonumber(L, 1);
    lua_pushnumber(L, digitalRead((int)pin));
    return 1;
}

static int l_analogRead(lua_State *L) {
    double pin = lua_tonumber(L, 1);
    lua_pushnumber(L, analogRead(pin));
    return 1;
}

static int l_analogWrite(lua_State *L) {
    double pin = lua_tonumber(L, 1);
    double value = lua_tonumber(L, 2);
    analogWrite((int)pin, (int)value);
    return 0;
}

static int l_millis(lua_State *L) {
    lua_pushnumber(L, millis());
    return 1;
}

static int l_delay(lua_State *L) {
    double length = lua_tonumber(L, 1);
    delay((long)length);
    return 0;
}

static int l_tone(lua_State *L) {
    double pin = lua_tonumber(L,1);
    double freq = lua_tonumber(L,2);
    tone((int)pin, (int)freq);
    return 0;
}

static int l_noTone(lua_State *L) {
    double pin = lua_tonumber(L, 1);
    noTone((int)pin);
    return 0;
}

static const struct luaL_reg arduinolib [] = {
    {"pinMode", l_pinMode},
    {"digitalWrite", l_digitalWrite},
    {"digitalRead", l_digitalRead},
    {"millis", l_millis},
    {"analogRead", l_analogRead},
    {"analogWrite", l_analogWrite},
    {NULL, NULL}  /* sentinel */
};

int luaopen_arduino(lua_State *L) {
    lua_register(L, "pinMode", l_pinMode);
    lua_register(L, "digitalWrite", l_digitalWrite);
    lua_register(L, "digitalRead", l_digitalRead);
    lua_register(L, "millis", l_millis);
    lua_register(L, "analogRead", l_analogRead);
    lua_register(L, "analogWrite", l_analogWrite);
    lua_register(L, "delay", l_delay);
    lua_register(L, "tone", l_tone);
    lua_register(L, "noTone", l_noTone);
}

#endif