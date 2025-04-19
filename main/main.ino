// ATmega328P_Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
// Arduino-Nano-Pinout : https://www.electronicshub.org/wp-content/uploads/2021/01/Arduino-Nano-Pinout.jpg

#include "defines.h"
#include "globals.h"


void setup(){

  setup_digital_pins();

  setup_timer1();

  setup_timer2();

}


void loop() {

  // read the buttons and update the settings
  read_buttons();

  // update attack and release
  update_AR();

  // update LFO_value
  update_LFO();

  // update OCR1A (dependencies: LFO)
  update_frequency();

  // update amp (dependencies: LFO, AR)
  update_amplitude();

  // update ampR and ampL (dependencies: LFO, amp)
  update_pan();

}


// Convert the sawtooth-wave into a square-wave.
// Is used by the LFO.
uint8_t square(uint8_t x){
  if(x>>7){return 255;}
  else    {return 0;}
}

