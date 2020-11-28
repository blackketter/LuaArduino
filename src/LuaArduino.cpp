#include "LuaArduino.h"
#include "Arduino.h"
#include "lua/arduino_functions.h"

// static class in and out objects
Stream* Lua::in = nullptr;
Print* Lua::out = nullptr;

// Allow printing (eg with Serial) using the stream operator
template<class T> inline Print& operator <<(Print &obj,     T arg) { obj.print(arg);    return obj; }
template<>        inline Print& operator <<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

Lua::Lua() {
  setStream(&Serial);
  initState();
}

Lua::Lua(Stream* s) {
  setStream(s);
  initState();
}

Lua::~Lua() {
  // destroy lua vm
  lua_close(L);
}

void Lua::reset() {
  // destroy lua vm
  lua_close(L);

  // create another one and initialize it
  initState();
}

void Lua::initState() {
  L = luaL_newstate(); /* opens Lua */
  if (L == 0) {
    return;
  }

  luaL_openlibs(L); /* opens the standard libraries */

  // Clear the stack
  lua_settop(L, 0);
  
  //open Arduino libraries
  luaopen_arduino(getState());
  
  //Add Arduino variables
  const char arduino_vars[] = 
  "LOW = 0\n"
  "HIGH = 1\n"
  "INPUT = 0\n"
  "OUTPUT = 1\n"
  "INPUT_PULLUP = 2\n";
  loadString(arduino_vars);

}

void Lua::loadString(String& s) {
  loadString(s.c_str());
}

void Lua::loadString(const char lbuff[]) {
  int error = 0;

  error = luaL_loadstring(L, lbuff);

  if (error) {
    out->print("Error ");
    out->print(error);
    out->print(" loading string: ");
    out->println(lbuff);
    out->println(lua_tostring(L, -1));
    lua_pop(L, 1); /* pop error message from the stack */
  }
  else {
    error = lua_pcall(L, 0, 0, 0);
    if (error) {
      out->print("Error ");
      out->print(error);
      out->println(" running:");
      out->println(lua_tostring(L, -1));
      lua_pop(L, 1); /* pop error message from the stack */
    }
  }
}

const char help_function[] = "function help ()\n"
"  print('===== Lua Help ======')\n"
"  print()\n"
"  print('RAM Usage: ')\n"
"  print(\"collectgarbage'count'\")\n"
"  print()\n"
"  print('Garbage collection: ')\n"
"  print(\"collectgarbage'collect'\")\n"
"  print()\n"
"  print('Ctrl-D to exec code')\n"
"  print()\n"
"end\n";

void Lua::help() {
  // Load global help() function
  loadString(help_function);

  // Exec help() lua function
  lua_getglobal(L, "help");
  lua_call(L, 0, 0);  // 0, 0 = #args, #retvals
}

// Compatibility functions for Serial output and SdFat file operations
extern "C" {

  int lua_compat_fopen(const char *filename) {
    Serial << "-- sd fopen: " << filename << "\n";
    return 0;
//    return (lua_temp_stdio_file.fopen(filename, "r") ? 1 : 0);

  }

  void lua_compat_fclose() {
    Serial << "-- sd fclose\n";
    //lua_temp_stdio_file.fclose();
  }

  int lua_compat_feof() {
    Serial << "-- sd feof\n";
    return 0;
    //return lua_temp_stdio_file.feof();
  }

  size_t lua_compat_fread(void* ptr, size_t size, size_t count) {
    Serial << "-- sd fread -- size: " << size << " count: " << count << "\n";
    return 0;
    //return lua_temp_stdio_file.fread(ptr, size, count);
  }

  int lua_compat_ferror() {
    Serial << "-- sd ferror\n";
    return 0;
    //return lua_temp_stdio_file.ferror();
  }

  void lua_compat_print(const char *s){
    Lua::getOut()->print(s);
  }

}
