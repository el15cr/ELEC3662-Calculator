#ifndef KEYPAD_H
#define KEYPAD_H

extern void keypadPortDInit(void);
extern void keypadPortEInit(void);
extern int keypadCheckRow(void);
extern void keypadSetColumn(int i);
extern unsigned char keypadRead(void);
extern char keypadDecodeKeyPress(unsigned char key_press);
extern void keypadCheckFlag(void);
extern char keypadGetKeyPress(void);
extern void keypadSetKeyPress(void);
extern void keypadHandler(void);
extern void keypadInit(void);

#endif
