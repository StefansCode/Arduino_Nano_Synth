#include "defines.h"
#include "globals.h"


void setup_digital_pins(){
//config. digital Pins as In-/Output
  
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);

  // Pin  3: PWM output to left speaker
  pinMode(3,OUTPUT);
  // Pin 11: PWM output to right speaker
  pinMode(11,OUTPUT);


  //config. the digital pins for Interupts
  PCICR  = 0;
  PCICR  |= B00000101;          //enable Interupts at Ports B and D
  PCMSK0 = 0;
  PCMSK0 |= B00000011;         //enable Interupts at Pin PB8, PB9, PB2, PB4 (D8, D9, D10, D12)
  PCMSK2 = 0;
  PCMSK2 |= B11110000;         //enable Interupts at Pin PD2, PD4 (D2, D4)
}


void setup_timer1(){
//config. timer1 for freq.

  TIMSK1 = 0;
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00001001;
  TIMSK1 |= B00000010;
}


void setup_timer2(){
//config. Timer2 for PWM

  TIMSK2 = 0;
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A |= B10100011;
  TCCR2B |= B00000001;
  TIMSK2 |= B00000001;
}


ISR(TIMER1_COMPA_vect){
// Interupt from timer1, when it reaches OCR1A.
// Updates the pulsewidth of the PWM-signal at pin 3 and 11.
  if(Saw_notSin){
    OCR2A = ampL*counter >> 8;
    OCR2B = ampR*counter++ >> 8;
  }
  else{
     OCR2A = ampL*sin_value[counter] >> 8;
     OCR2B = ampR*sin_value[counter++] >> 8;
  }
}


ISR(TIMER2_OVF_vect){
// Interupt from timer2, when it overflowes.
// Increase the LFO-counter which is essentially a slow timer for the LFO.
// Increase and decrease the envelope-counter depending on the
// values of attack and release

  LFO_counter0++;

  if(PLAY_ispressed && !((Env_counter0+attack)>>31)){
    Env_counter0 += attack;
  }
  else if(!(PLAY_ispressed) && !((Env_counter0-release)>>31)){
    Env_counter0 -= release;
  }
}


// Interupt at Port B
ISR(PCINT0_vect){
  button_interupt_flag = 1;
}


// Interupt at Port D
ISR(PCINT2_vect){
  button_interupt_flag = 1;
}
