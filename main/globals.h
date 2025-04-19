#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

extern float    help;                        // a help variable
extern volatile uint8_t     button_interupt_flag;
extern uint8_t  Settings;                    // used as a bitarray to hold the current settings
extern uint8_t  counter;                     // counter for Timer2/the soundwave
extern uint16_t amp;                         // A0 for amplitued/volume
extern uint16_t ampR;                        // right speaker
extern uint16_t ampL;                        // left  speaker
//directly written to OCR1A                   // A7 for frequency/pitch
//LFO_type is direckly read from the registers
extern uint8_t  LFO_freq;                    // A2 LFO frequency
extern uint8_t  LFO_depth;                   // A3 LFO amplitude/depth
extern uint32_t LFO_counter0;                // LFO_counter++ when Timer2 overflowes (essentially a slow Timer which depends on Timer2)
extern uint8_t  LFO_value;                   // current LFO value depents on LFO_counter, LFO_freq and LFO_depth
extern uint32_t attack;                      // A6 for how long the amp rises if a Note is played
extern uint32_t release;                     // A1 for how long the amp falls if a Note is played
extern uint32_t Env_counter0;                // Env_counter0++ when Note is played and Timer2 overflowes
                                      // Env_counter0-- when no Note is played and Timer2 overflowes

// A4 is for pan
// open wire at A5 is still unused


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


#endif //GLOBALS_H