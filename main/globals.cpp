#include "defines.h"
#include "globals.h"

extern float    help = 0;                        // a help variable
extern uint8_t  Settings = 35;    //35==B00100011    // used as a bitarray to holds butten values
extern uint8_t  counter = 0;                     // counter for Timer2/the soundwave
extern uint16_t amp = 0;                         // A0 for amplitued/volume
extern uint16_t ampR = 0;                        // right speaker
extern uint16_t ampL = 0;                        // left  speaker
//directly written to OCR1A                      // A7 for frequency/pitch

//variables for the LFO
//LFO_type is direckly read from the registers
extern uint8_t  LFO_freq = 0;                    // A2 LFO frequency
extern uint8_t  LFO_depth = 0;                   // A3 LFO amplitude/depth
extern uint32_t LFO_counter0 = 0;                // LFO_counter++ when Timer2 overflowes (essentially a slow Timer which depends on Timer2)
extern uint8_t  LFO_value = 0;                   // current LFO value depents on LFO_counter, LFO_freq and LFO_depth

// Variables for the AR-envelope
extern uint32_t attack = 0;                      // A6 for how long the amp rises if a Note is played
extern uint32_t release = 0;                     // A1 for how long the amp falls if a Note is played
extern uint32_t Env_counter0 = 0;  