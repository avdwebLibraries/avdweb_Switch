#include "Arduino.h"
#include "avdweb_Switch.h"

const byte toggleSwitchpin = 10;
const byte multiresponseButtonpin = 9;

Switch toggleSwitch = Switch(toggleSwitchpin);
Switch multiresponseButton = Switch(multiresponseButtonpin);

void buttonCallbackFunction(void* s) {
  Serial.print("Button: ");
  Serial.println((char*)s);
}

void toggleCallbackFunction(void* s) {
  Serial.print("Toggle: ");
  Serial.println((char*)s);
}

void setup()
{
  Serial.begin(9600);
  toggleSwitch.setPushedCallback(&toggleCallbackFunction, (void*)"turned on");
  toggleSwitch.setReleasedCallback(&toggleCallbackFunction, (void*)"turned off");

  multiresponseButton.setLongPressCallback(&buttonCallbackFunction, (void*)"long press");
  multiresponseButton.setDoubleClickCallback(&buttonCallbackFunction, (void*)"double click");
  multiresponseButton.setSingleClickCallback(&buttonCallbackFunction, (void*)"single click");
}

void loop()
{
  toggleSwitch.poll();
  multiresponseButton.poll();
}
