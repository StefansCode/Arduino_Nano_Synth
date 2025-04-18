
// Acsess the digital Pins directly
#define Btn1_ispressed (!((PIND>>4)%2))     // D4
#define Btn2_ispressed (!((PINB>>4)%2))     // D12
#define Btn3_ispressed (!((PINB>>2)%2))     // D10
#define Btn4_ispressed (!((PIND>>2)%2))     // D2
#define Btn5_ispressed (!((PINB>>1)%2))     // D9
#define Btn6_ispressed (!((PINB)%2))        // D8

// give each Bit of the Byte "BTNS" a Name
#define Saw_notSin     ((BTNS)%2)
#define Scale_notFree  ((BTNS>>1)%2)
#define LFO_toAmp      ((BTNS>>2)%2)
#define LFO_toFreq     ((BTNS>>3)%2)
#define LFO_toPan      ((BTNS>>4)%2)
#define LFO_typeSin    ((BTNS>>5)%2)
#define LFO_typeSaw    ((BTNS>>6)%2)
#define LFO_typeSqare  ((BTNS>>7)%2)