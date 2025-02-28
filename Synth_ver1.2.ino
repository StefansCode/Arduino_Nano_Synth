// ATmega328P_Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
// Arduino-Nano-Pinout : https://www.electronicshub.org/wp-content/uploads/2021/01/Arduino-Nano-Pinout.jpg


#define Btn1_ispressed (!((PIND>>4)%2))
#define Btn2_ispressed (!((PINB>>4)%2))
#define Btn3_ispressed (!((PINB>>2)%2))
#define Btn4_ispressed (!((PIND>>2)%2))
#define Btn5_ispressed (!((PINB>>1)%2))
#define Btn6_ispressed (!((PINB)%2))

#define Saw_notSin     ((BTNS)%2)
#define Scale_notFree  ((BTNS>>1)%2)
#define LFO_toAmp      ((BTNS>>2)%2)
#define LFO_toFreq     ((BTNS>>3)%2)
#define LFO_toPan      ((BTNS>>4)%2)
#define LFO_typeSin    ((BTNS>>5)%2)
#define LFO_typeSaw    ((BTNS>>6)%2)
#define LFO_typeSqare  ((BTNS>>7)%2)

// 256 values of sin between [0:2PI] 
const uint8_t sin_value[] = {128, 131, 134, 137, 140, 143, 146, 149, 152, 156, 159, 162, 165, 168, 171, 174, 176, 179, 182, 185, 188, 191, 193,
                             196, 199, 201, 204, 206, 209, 211, 213, 216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 237, 239, 240, 242, 
                             243, 245, 246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 
                             255, 254, 254, 253, 252, 252, 251, 250, 249, 248, 247, 246, 245, 243, 242, 240, 239, 237, 236, 234, 232, 230, 228, 
                             226, 224, 222, 220, 218, 216, 213, 211, 209, 206, 204, 201, 199, 196, 193, 191, 188, 185, 182, 179, 176, 174, 171, 
                             168, 165, 162, 159, 156, 152, 149, 146, 143, 140, 137, 134, 131, 127, 124, 121, 118, 115, 112, 109, 106, 103, 99, 
                             96, 93, 90, 87, 84, 81, 79, 76, 73, 70, 67, 64, 62, 59, 56, 54, 51, 49, 46, 44, 42, 39, 37, 35, 33, 31, 29, 27, 25, 
                             23, 21, 19, 18, 16, 15, 13, 12, 10, 9, 8, 7, 6, 5, 4, 3, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 3, 
                             4, 5, 6, 7, 8, 9, 10, 12, 13, 15, 16, 18, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 42, 44, 46, 49, 51, 54, 56, 59, 
                             62, 64, 67, 70, 73, 76, 79, 81, 84, 87, 90, 93, 96, 99, 103, 106, 109, 112, 115, 118, 121, 124
};

// values for OCR1A, so the resulting frequency is part of the C-scale
const uint16_t C_scale[] = {1901, 1695, 1530, 1426, 1280, 1140, 1012,
                            965, 859, 765, 721, 640, 570, 510, 
                            478, 426, 380, 358, 320, 285, 254,
                            239, 213, 190, 179, 160, 142, 127,
                            119, 106, 95, 89, 80, 71, 63, 
                            59, 53, 47, 44, 40, 35, 31,
                            29, 26, 23, 22, 20
};

// convert the sawtooth-wave into a square-wave
uint8_t square(uint8_t x){
  if(x>>7){return 255;}
  else    {return 0;}
}

/*
// Low-pass-Filter
uint8_t old_sum = 0;
void LPF(uint8_t x){
  oldsum = 
}
*/

void setup()
{
  //config. digital Pins as In-/Output
  pinMode(4, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  pinMode(3,OUTPUT);
  pinMode(11,OUTPUT);

  //config. Inputs for Interupts
  PCICR  = 0;
  PCICR  |=B00000101;          //enable Interupts at Ports B and D
  PCMSK0 = 0;
  PCMSK0 |= B00010111;         //enable Interupts at Pin PB8, PB9, PB2, PB4 (D8, D9, D10, D12)
  PCMSK2 = 0;
  PCMSK2 |= B00010100;         //enable Interupts at Pin PD2, PD4 (D2, D4)


  //config. Timer2 for PWM
  TIMSK2 = 0;
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2A |= B10100011;
  TCCR2B |= B00000001;
  TIMSK2 |= B00000001;

  //config. Timer1 for freq.
  TIMSK1 = 0;
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00001001;
  TIMSK1 |= B00000010;

}


float    help;                        // a help variable
uint8_t  BTNS= B00100011;             // holds butten values

uint8_t  counter = 0;                 // counter for Timer2/the soundwave
uint16_t amp;                         // A0 for amplitued/volume
uint16_t ampR;                        // right speaker
uint16_t ampL;                        // left  speaker
//directly written to OCR1A           // A7 for frequency/pitch

//LFO_type is direckly read from the registers
uint8_t  LFO_freq;                    // A2 LFO frequency
uint8_t  LFO_depth;                   // A3 LFO amplitude/depth
uint32_t LFO_counter0;                // LFO_counter++ when Timer2 overflowes (essentially a slow Timer which depends on Timer2)
uint8_t  LFO_value;                   // current LFO value depents on LFO_counter, LFO_freq and LFO_depth

uint32_t attack;                      // A6 for how long the amp rises if a Note is played
uint32_t release;                     // A1 for how long the amp falls if a Note is played
uint32_t Env_counter0;                // Env_counter0++ when Note is played and Timer2 overflowes
                                      // Env_counter0-- when no Note is played and Timer2 overflowes

// A4 is for pan
// open wire at A5 is still unused




void loop() {


// read data for attack and release
  attack  = uint32_t(pow(2, map(analogRead(A6), 0, 1023, 22, 8)));
  release = uint32_t(pow(2, map(analogRead(A1), 0, 1023, 22, 8)));
//


// read data for the LFO and update it
  LFO_depth = analogRead(A3)>>2;                        //read A3 range 0-255
  LFO_freq  = map(analogRead(A2),0,1023, 4, 15);        //read A2: range 7 - 14  =>  LFO_freq range 0.015 - 1.9 Hz
  
  LFO_value = LFO_counter0 >> LFO_freq;
  if(LFO_typeSin){
    LFO_value = (sin_value[LFO_value] * LFO_depth) >> 8;
  }
  if(LFO_typeSaw){
    LFO_value = (LFO_value * LFO_depth) >> 8;
  }
  if(LFO_typeSqare){
    LFO_value = (square(LFO_value) * LFO_depth) >> 8;
  }
//


// read data for the frequence/pitch and update it
  if(LFO_toFreq){
    // if LFO is set to freq (5V-cabel is at Pin7) modifi OCR1A it with LFO
    help = pow(2, float((LFO_depth>>1)-LFO_value)/256);
    if(Scale_notFree){
      OCR1A = uint16_t(help*float(C_scale[map(analogRead(A7),0,1024,14,27)])) ;
    }
    else{
      OCR1A = uint16_t(help*float(max(119,(map(analogRead(A7),0,1024,475,100))))) ;
    }
  }
  else{
    if(Scale_notFree){
      // If butten 2 is not set, lock pitch to C scale
      OCR1A = C_scale[map(analogRead(A7),0,1024,14,27)];
      }
    else{
      // If butten 2 is set, free pitch
      OCR1A = max(119,(map(analogRead(A7),0,1024,475,100)));  //read A7: range 110 - 490  =>  pitch range 131 - 520 Hz (C3 - C5)
    }
  }
//


// read data for the amplitude/volume and update it
  amp = ((Env_counter0>>23)*max(0,map(analogRead(A0)>>2,15,255,0,255)))>>8;     // set range to 0-255 and apply envelope
  // if LFO is set to amp
  if(LFO_toAmp){
    // modifi amp it with LFO
    amp = amp - ((amp * LFO_value) >> 8);
  }
//


// read data for pan / left/right
  // if 5V-cabel is at Pin5
  if(LFO_toPan){
    ampR = amp - ((amp * LFO_value) >> 8);
    ampL = amp - ((amp * (LFO_depth-LFO_value)) >> 8);
  }
  else{
    ampR = (amp*(analogRead(A4)>>2))>>8;
    ampL = (amp*(255-(analogRead(A4)>>2)))>>8;
  }
//

}




// Interupt at Port B
ISR(PCINT0_vect){
  if(Btn3_ispressed){
    //right button (at D10) is pressed
    BTNS ^= 1 << 1;
  }
  else if(Btn5_ispressed){
    //update LFO destination
    if(LFO_toAmp){
      BTNS ^= 3 << 2;
    }
    else if(LFO_toFreq){
      BTNS ^= 3 << 3;
    }
    else if(LFO_toPan ){
      BTNS ^= 1 << 4;
    }
    else{
      BTNS ^= 1 << 2;
    }
  }
  else if(Btn6_ispressed){
    //update LFO waveForm
    if(LFO_typeSin){
      BTNS ^= 3 << 5;
    }
    else if(LFO_typeSaw){
      BTNS ^= 3 << 6;
    }
    else if(LFO_typeSqare){
      BTNS ^= 1 << 7;
      BTNS ^= 1 << 5;
    }
  }
}



// Interupt at Port D
ISR(PCINT2_vect){
  if(Btn4_ispressed){
    //right button (at D2) is pressed
    BTNS ^= 1 ;
  }
  /*
  if((PIND>>4)%2){
    //right button (at D4) is pressed
    BTNS ^= 1 << 3;
  }
  */
}


// Interupt for freq
ISR(TIMER1_COMPA_vect){
  if(Saw_notSin){
    OCR2A = ampL*counter >> 8;
    OCR2B = ampR*counter++ >> 8;
  }
  else{
     OCR2A = ampL*sin_value[counter] >> 8;
     OCR2B = ampR*sin_value[counter++] >> 8;
  }
}


// Interupt for LFO and envelope
ISR(TIMER2_OVF_vect){

  LFO_counter0++;

  if(Btn1_ispressed && !((Env_counter0+attack)>>31)){
    Env_counter0 += attack;
  }
  else if(!(Btn1_ispressed) && !((Env_counter0-release)>>31)){
    Env_counter0 -= release;
  }
}




