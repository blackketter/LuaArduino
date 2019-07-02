#include "LuaArduino.h"
#include "Arduino.h"

#if defined(ARDUINO_AVR_LEONARDO)
  // leonardo pro micro has Rx LED on pin 17
  #define LED (17)
  #define LEDON (LOW)
#elif defined(ARDUINO_ESP32_DEV)
  #define LED (5)
  #define LEDON (HIGH)
#elif defined(ARDUINO_LOLIN32)
  #define LED (22)
  #define LEDON (HIGH)
#elif defined(ESP8266_WEMOS_D1MINI)
  #define LED (2)
  #define LEDON (LOW)
#else
  // Pin 13 has an LED connected on most Arduino boards:
  #define LED (13)
  #define LEDON (LOW)
#endif

#define LEDOFF (!LEDON)

Lua* lua;

void printPrompt() {
  Serial.print("\n>> ");
}

void setup(void) {

  // Enable printf/sprintf to print floats for Teensy
  asm(".global _printf_float");

  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  delay(2000);

  lua = new Lua;
  if (lua && lua->getState()) {
    lua->help();

    printPrompt();
  } else {
    Serial.print("Lua Failed To Allocate!");
  }
}


void loop() {

  static char repl_buffer[1024];
  static uint16_t buff_position = 0;

  if (Serial.available() > 0) {
    char new_char = Serial.read();
    if ((new_char >= 32 && new_char <= 127) // printable character
        || new_char == '\n' // line break
        || new_char == 4    // EOT end of transmission
        || new_char == 8    // backspace
        || new_char == 9    // tab
        ) {
      repl_buffer[buff_position] = new_char;
      // echo new char
      Serial.write(new_char);
//      Serial.write('.');
      buff_position++;
    }
    // TODO handle escape sequences 27 (and arrow keys)
  }

  // if no characters received skip the rest of the loop
  if (buff_position != 0) {
    // if backspace was pressed
    if (repl_buffer[buff_position-1] == 8) {
      if (buff_position == 1)
        // just remove the backspace character
        buff_position--;
      else
        // remove both the backspace character and the previously entered character
        buff_position = buff_position - 2;
    }
    // if EOT end of transmission == 4 (Ctrl-D in miniterm.py)
    else if (repl_buffer[buff_position-1] == 4) {
      // set the last character to the null char (should overwrite the EOT)
      repl_buffer[buff_position-1] = '\0';

//    Serial.printf( "\nGot: '%s'\n", repl_buffer);
      if (lua) {
        lua->loadString(repl_buffer);
      } else {
        Serial.println("No Lua Interpreter!");
      }
      // reset buffer index
      buff_position = 0;

      printPrompt();
    }
  }

  // blink the LED
  if ((millis() / 250) % 2){
    digitalWrite(LED, LEDON);
  } else {
    digitalWrite(LED, LEDOFF);
  }

}
