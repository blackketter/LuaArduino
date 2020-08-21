#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <LuaArduino.h>
#include <lua/lapi.h>

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
}