#ifndef MATH_H
#define MATH_H

extern double mathGetResult(void); 
extern void mathShiftNumberSigns(int start, int end);
extern int mathGetSignsLenght(void);
extern int mathGetNumbersLenght(void);
extern void mathPerformOperation(char sign, int point, int lenght);
extern void mathPerformCalculation(void);
extern int mathCheckDecimalPoint(int start, int end, char *buffer);
extern double mathHexToDouble(int start, int end, char *buffer);
extern void mathSortBuffer(char *buffer);
extern void mathInit(void);

#endif