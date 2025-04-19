#include "defines.h"
#include "globals.h"


void setup_digital_pins(){
//config. digital Pins as In-/Output
  
  pinMode(4, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  // Pin  3: PWM output to left speaker
  pinMode(3,OUTPUT);
  // Pin 11: PWM output to right speaker
  pinMode(11,OUTPUT);


  //config. the digital pins for Interupts
  PCICR  = 0;
  PCICR  |=B00000101;          //enable Interupts at Ports B and D
  PCMSK0 = 0;
  PCMSK0 |= B00010111;         //enable Interupts at Pin PB8, PB9, PB2, PB4 (D8, D9, D10, D12)
  PCMSK2 = 0;
  PCMSK2 |= B00010100;         //enable Interupts at Pin PD2, PD4 (D2, D4)
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

  if(Btn1_ispressed && !((Env_counter0+attack)>>31)){
    Env_counter0 += attack;
  }
  else if(!(Btn1_ispressed) && !((Env_counter0-release)>>31)){
    Env_counter0 -= release;
  }
}


// Interupt at Port B
ISR(PCINT0_vect){
  if(Btn3_ispressed){
    //right button (at D10) is pressed
    Settings ^= 1 << 1;
  }
  else if(Btn5_ispressed){
    //update LFO destination
    if(LFO_toAmp){
      Settings ^= 3 << 2;
    }
    else if(LFO_toFreq){
      Settings ^= 3 << 3;
    }
    else if(LFO_toPan ){
      Settings ^= 1 << 4;
      Settings ^= 1 << 2;
    }
  }
  else if(Btn6_ispressed){
    //update LFO waveForm
    if(LFO_typeSin){
      Settings ^= 3 << 5;
    }
    else if(LFO_typeSaw){
      Settings ^= 3 << 6;
    }
    else if(LFO_typeSqare){
      Settings ^= 1 << 7;
      Settings ^= 1 << 5;
    }
  }
}


// Interupt at Port D
ISR(PCINT2_vect){
  if(Btn4_ispressed){
    //right button (at D2) is pressed
    Settings ^= 1 ;
  }
  /*
  if((PIND>>4)%2){
    //right button (at D4) is pressed
    Settings ^= 1 << 3;
  }
  */
}