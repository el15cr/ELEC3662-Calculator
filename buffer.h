#ifndef BUFFER_H
#define BUFFER_H

extern char buffer[16]; 
extern void bufferShift(int start, char replace);
extern int bufferCheckError(void);
extern void bufferWriteAscii(char ascii);
extern void bufferReadLcd(unsigned long i, int row);
extern void bufferWriteRead(int row, char key); 
extern int bufferGetLenght(void);
extern void bufferPrintResults(double result); 
extern void bufferInit(void);

#endif
