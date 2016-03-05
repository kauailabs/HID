/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  Gamepad example
  Press a button and demonstrate Gamepad actions

  You can also use Gamepad1,2,3 and 4 as single report.
  This will use 1 endpoint for each gamepad.

  See HID Project documentation for more infos
  https://github.com/NicoHood/HID/wiki/Gamepad-API
*/

#include "HID-Project.h"

const int pinLed = LED_BUILTIN;
const int pinButton = 2;

void setup() {
  pinMode(pinLed, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);
  Serial.begin(57600);
  // Sends a clean report to the host. This is important on any Arduino type.
  GamepadWithLEDs1.begin();
}

void loop() {
  if (!digitalRead(pinButton)) {
    digitalWrite(pinLed, HIGH);

    // Press button 1-32
    static uint8_t count = 0;
    count++;
    if (count == 33) {
      GamepadWithLEDs1.releaseAll();
      count = 0;
    }
    else
      GamepadWithLEDs1.press(count);

    // Move x/y Axis to a new position (16bit)
    GamepadWithLEDs1.xAxis(random(0xFFFF));
    GamepadWithLEDs1.yAxis(random(0xFFFF));

    // Go through all dPad positions
    // values: 0-8 (0==centered)
    static uint8_t dpad1 = GAMEPAD_DPAD_CENTERED;
    GamepadWithLEDs1.dPad1(dpad1++);
    if (dpad1 > GAMEPAD_DPAD_UP_LEFT)
      dpad1 = GAMEPAD_DPAD_CENTERED;

    static int8_t dpad2 = GAMEPAD_DPAD_CENTERED;
    GamepadWithLEDs1.dPad2(dpad2--);
    if (dpad2 < GAMEPAD_DPAD_CENTERED)
      dpad2 = GAMEPAD_DPAD_UP_LEFT;

    // Functions above only set the values.
    // This writes the report to the host.
    GamepadWithLEDs1.write();

    // Simple debounce
    delay(300);
    digitalWrite(pinLed, LOW);
  } else {
    static uint8_t curr_button = 0;
    GamepadWithLEDs1.releaseAll();
    GamepadWithLEDs1.press(curr_button);
    GamepadWithLEDs1.write();
    curr_button = curr_button + 1;
    if ( curr_button > 32 ) {
      curr_button = 0;
    }    
    Serial.print("Len:  ");
    Serial.print(GamepadWithLEDs1.getLen());
    Serial.print("Leds:  ");
    Serial.println(GamepadWithLEDs1.getLeds(),HEX);
    delay(100);
  }
}
