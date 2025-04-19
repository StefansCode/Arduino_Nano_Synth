#include "defines.h"
#include "globals.h"


void update_AR(){
// read data for attack and release and update it
 
  attack  = uint32_t(pow(2, map(analogRead(ATTACK_POTI), 0, 1023, 22, 8)));
  release = uint32_t(pow(2, map(analogRead(RELEASE_POTI), 0, 1023, 22, 8)));
}


void update_LFO(){
// read data for the LFO and update it
  
  LFO_depth = analogRead(LFO_DEPTH_POTI)>>2;                       //read A3 range 0-255
  LFO_freq  = map(analogRead(LFO_FREQ_POTI),0,1023, 4, 15);        //read A2: range 7 - 14  =>  LFO_freq range 0.015 - 1.9 Hz
  
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
}


void update_frequency(){
// read data for the frequence/pitch and update it
  
  if(LFO_toFreq){
    // if LFO is set to freq modifi OCR1A with LFO
    help = pow(2, float((LFO_depth>>1)-LFO_value)/256);
    if(Scale_notFree){
      OCR1A = uint16_t(help*float(C_scale[map(analogRead(FREQ_POTI),0,1024,14,27)])) ;
    }
    else{
      OCR1A = uint16_t(help*float(max(119,(map(analogRead(FREQ_POTI),0,1024,475,100))))) ;
    }
  }
  else{
    if(Scale_notFree){
      // If butten 2 is not set, lock pitch to C scale
      OCR1A = C_scale[map(analogRead(FREQ_POTI),0,1024,14,27)];
      }
    else{
      // If butten 2 is set, free pitch
      OCR1A = max(119,(map(analogRead(FREQ_POTI),0,1024,475,100)));  //read A7: range 110 - 490  =>  pitch range 131 - 520 Hz (C3 - C5)
    }
  }
}


void update_amplitude(){
// read data for the amplitude/volume and update it

  amp = ((Env_counter0>>23)*max(0,map(analogRead(AMP_POTI)>>2,15,255,0,255)))>>8;     // set range to 0-255 and apply envelope
  if(LFO_toAmp){
    // if LFO is set to amp modifi amp it with LFO
    amp = amp - ((amp * LFO_value) >> 8);
  }
}


void update_pan(){
// read data for pan / left/right
  
  if(LFO_toPan){
    // if LFO is set to pan modifi ampR and ampL it with LFO
    ampR = amp - ((amp * LFO_value) >> 8);
    ampL = amp - ((amp * (LFO_depth-LFO_value)) >> 8);
  }
  else{
    ampR = (amp*(analogRead(A4)>>2))>>8;
    ampL = (amp*(255-(analogRead(A4)>>2)))>>8;
  }
}


void read_buttons(){
  
  if(button_interupt_flag){
    
    button_interupt_flag = 0;
    
    if(LFO_DEST_ispressed){
      //update LFO destination
      if(LFO_toAmp){
        Settings ^= 3 << 2;
      }
      else if(LFO_toFreq){
        Settings ^= 3 << 3;
      }
      else if(LFO_toPan ){
        Settings ^= 1 << 4;
      }
      else{
        Settings ^= 1 << 2;
      }
    }
    if(LFO_TYPE_ispressed){
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
    if(CHANGE_WAVE_ispressed){
      Settings ^= 1 ;
    }
    if(FIX_C_SCALE_ispressed){
      Settings ^= 1 << 1;
    }
  }
}
