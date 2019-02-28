#ifndef PLL_H
#define PLL_H

extern void PLLInit(void);
extern void SysTick_Wait(unsigned long delay);
extern void Systick_Wait1ms(unsigned long delay);
extern void Systick_Wait1us(unsigned long delay);

#endif
