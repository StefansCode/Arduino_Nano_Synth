#ifndef DEFINES_H
#define DEFINES_H


// Rename the analog pins.
// These can be changed depending on 
// how the hardware is wired.
#define AMP_POTI        A6
#define FREQ_POTI       A5
#define PAN_POTI        A4
#define ATTACK_POTI     A3
#define RELEASE_POTI    A2
#define LFO_FREQ_POTI   A1
#define LFO_DEPTH_POTI  A0

// Acsess the digital pins directly.
// These should not be changend, otherwise the 
// IRS-functions must be changed.
#define PLAY_ispressed        ((PIND>>4)%2)   // D4
#define Btn2_ispressed        ((PIND>>5)%2)   // D5
#define FIX_C_SCALE_ispressed ((PIND>>6)%2)   // D6
#define CHANGE_WAVE_ispressed ((PIND>>7)%2)   // D7
#define LFO_TYPE_ispressed    ((PINB)%2)      // D8
#define LFO_DEST_ispressed    ((PINB>>1)%2)   // D9

// Give each Bit of the Byte "Settings" a Name
#define Saw_notSin     ((Settings)%2)
#define Scale_notFree  ((Settings>>1)%2)
#define LFO_toAmp      ((Settings>>2)%2)
#define LFO_toFreq     ((Settings>>3)%2)
#define LFO_toPan      ((Settings>>4)%2)
#define LFO_typeSin    ((Settings>>5)%2)
#define LFO_typeSaw    ((Settings>>6)%2)
#define LFO_typeSqare  ((Settings>>7)%2)


#endif //DEFINES_H