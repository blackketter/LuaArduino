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

    // execute a given string of Lua code
    void loadString(const char* s);
    void loadString(String& s);

    // set the input and output streams for the Lua interpreter
    void setStream(Stream* s) { setOut(s); setIn(s); };
    void setOut(Print* p) { out = p; };
    void setIn(Stream* s) { in = s; };
    static Print* getOut() { return out; };
    static Stream* getIn() { return in; };

    // lua_State accessor
    lua_State* getState() { return L; };

    // print out help information to the default stream
    void help();

  private:
    void initState();

    lua_State *L = nullptr;
    static Stream* in;
    static Print* out;
};

#endif
