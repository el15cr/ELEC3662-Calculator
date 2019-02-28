#ifndef LED_H
#define LED_H

extern void lcdPortAInit(void);
extern void lcdPortBInit(void);
extern void lcdENPluse(void);
extern void lcdWriteCommand(unsigned char c);
extern void lcdScreenClear(void);
extern void lcdWriteData(unsigned char c);
extern void lcdGoto(unsigned char address);
extern void lcdGotoRowColoum(unsigned int row, unsigned int coloumn);
extern void lcdSetCGRAM(unsigned char address);
extern void lcdSetCustomCharacter(unsigned char address, unsigned char custom_char[]);
extern void lcdDisplayCustomCharacter(unsigned char address, int row, int column);
extern void lcdShiftLeft(void);
extern void lcdShiftRight(void); 
extern void lcdShiftStop(void);
extern void lcdReturnHome(void);
extern void lcdRamString(char *string, int shift, char key);
extern void lcdInit(void);

#endif
