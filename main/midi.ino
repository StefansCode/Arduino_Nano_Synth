#include "defines.h"
#include <avr/wdt.h>

uint8_t channel = 0;

bool midi_is_setup = false;

void setup_midi(){
    Serial.begin(38400); // MIDI-Standard-Baudrate
    midi_is_setup = true;
}

void set_channel(uint8_t new_channel){
    channel = new_channel;
}

void sendNoteOn(byte note, byte velocity) {
    Serial.write(0x90 | (channel & 0x0F)); // Note On, Kanal
    Serial.write(note & 0x7F);             // Note-Nummer
    Serial.write(velocity & 0x7F);         // Velocity
}
  
void sendNoteOff(byte note, byte velocity) {
    Serial.write(0x80 | (channel & 0x0F)); // Note Off, Kanal
    Serial.write(note & 0x7F);
    Serial.write(velocity & 0x7F);
}

void midi_loop(){

  // if the swich is turned again reset the Arduino
  if(midi_is_setup && digitalRead(!SWICH_PIN)){
    wdt_enable(WDTO_15MS); // set Watchdog to 15ms
    while (1) {}           // loop till Watchdog resets the arduino
  }
  else if(!midi_is_setup && digitalRead(SWICH_PIN)){
    setup_midi();
  }
  else if(midi_is_setup){
    sendNoteOn(60, 100); // C4, Velocity 100, Kanal 1
    delay(500);          // Halte die Note 500ms
    sendNoteOff(60, 0);  // C4 aus, Velocity 0, Kanal 1
    delay(1000); 
  }
}