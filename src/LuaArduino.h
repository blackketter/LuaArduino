#ifndef _LuaArduino_
#define _LuaArduino_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"

#include "Stream.h"

class Lua {
  public:
    Lua();
    Lua(Stream* s);
    ~Lua();

    //  reset the Lua interpreter, freeing up all loaded code and data
    void reset();

    void loadString(const char* s); // execute a given string of Lua code
    void loadString(String& s);

    // set the input and output streams for the Lua interpreter
    void setOut(Print* p) { out = p; };
    void setIn(Stream* s) { in = s; };
    void setStream(Stream* s) { setOut(s); setIn(s); };

    lua_State* getState() { return L; };

  private:
    void initState();

    lua_State *L = nullptr;
    Stream* in = nullptr;
    Print* out = nullptr;
};

#endif
