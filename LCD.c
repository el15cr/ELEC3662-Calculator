// Port A defines
#define GPIO_PORTA_DATA_R  	(*((volatile unsigned long *) 0x400043FC))
#define GPIO_PORTA_DIR_R   	(*((volatile unsigned long *) 0x40004400))
#define GPIO_PORTA_AFSEL_R  (*((volatile unsigned long *) 0x40004420))
#define GPIO_PORTA_PUR_R  	(*((volatile unsigned long *) 0x40004510))
#define GPIO_PORTA_DEN_R   	(*((volatile unsigned long *) 0x4000451C))
#define GPIO_PORTA_AMSEL_R  (*((volatile unsigned long *) 0x40004528))
#define GPIO_PORTA_PCTL_R   (*((volatile unsigned long *) 0x4000452C))
// Port B defines
#define GPIO_PORTB_DATA_R  	(*((volatile unsigned long *) 0x400053FC))
#define GPIO_PORTB_DIR_R   	(*((volatile unsigned long *) 0x40005400))
#define GPIO_PORTB_AFSEL_R  (*((volatile unsigned long *) 0x40005420))
#define GPIO_PORTB_PUR_R  	(*((volatile unsigned long *) 0x40005510))
#define GPIO_PORTB_DEN_R   	(*((volatile unsigned long *) 0x4000551C))
#define GPIO_PORTB_AMSEL_R  (*((volatile unsigned long *) 0x40005528))
#define GPIO_PORTB_PCTL_R   (*((volatile unsigned long *) 0x4000552C))
// Misc defines
#define SYSCTL_RCGC2_R      (*((volatile unsigned long *) 0x400FE108))
// Bit Specific Ports 
#define lcdRS 							(*((volatile unsigned long *) 0x40004020)) // RS - PA3
#define lcdEN 							(*((volatile unsigned long *) 0x40004010)) // EN - PA2
#define lcdDB 							(*((volatile unsigned long *) 0x400050F0)) // DB4 – PB2 , DB5 – PB3 , DB6 – PB4 , DB7 – PB5
// Includes
#include "PLL.h"

// Function Definitions 

/** Initialises Port A
*/
void lcdPortAInit(void){
		volatile unsigned long delay;
  	SYSCTL_RCGC2_R |= 0x00000001;     // 1) Port A clock
  	delay = SYSCTL_RCGC2_R;           // delay        
  	GPIO_PORTA_AMSEL_R &= 0x00;       // 2) disable analog function
  	GPIO_PORTA_PCTL_R &= 0x00000000;  // 3) GPIO clear bit PCTL  
  	GPIO_PORTA_DIR_R |= 0x0C;        	// 4) PA2 - PA3 output, 
  	GPIO_PORTA_AFSEL_R &= 0x00;       // 5) no alternate function
  	GPIO_PORTA_PUR_R &= 0x00;         // disable pullup resistors on PA2 - PA3       
  	GPIO_PORTA_DEN_R |= 0x0C;         // 6) enable digital pins PA2 - PA3
}

/** Initialises Port B
*/
void lcdPortBInit(void){
		volatile unsigned long delay;
  	SYSCTL_RCGC2_R |= 0x0000002;     	// 1) Port B clock
  	delay = SYSCTL_RCGC2_R;           // delay        
  	GPIO_PORTB_AMSEL_R &= 0x00;       // 2) disable analog function
  	GPIO_PORTB_PCTL_R &= 0x00000000;  // 3) GPIO clear bit PCTL  
  	GPIO_PORTB_DIR_R |= 0x3C;        	// 4) PD2 - PD5 output, 
  	GPIO_PORTB_AFSEL_R &= 0x00;       // 5) no alternate function
  	GPIO_PORTB_PUR_R &= 0x00;         // disable pullup resistors on PD2 - PD5       
  	GPIO_PORTB_DEN_R |= 0x3C;         // 6) enable digital pins PD2 - PD5
}

/** Pluse enable
*/
void lcdENPluse(void){
	// Set enable on
	lcdEN = 0x04;
	// Wait for 500x10^-9 (more than 450ns(40/80x10^6))
	SysTick_Wait(40); 
	// Set enable off
	lcdEN = 0x00; 
}

/** Writes a command to the LCD. Bit shifts as LCD set to 4 bit
*@param The command that needs to be written 
*/
void lcdWriteCommand(unsigned char c){
	// Set RS to 0 to send command
	lcdRS = 0x00; 
	Systick_Wait1us(40); 
	// Shift high nibble left 2 and mask 
	lcdDB = (c >> 2) & 0x3C;
	lcdENPluse(); 
	// Pluse EN and wait
	Systick_Wait1ms(5); 
	// Shift low nibble right 2 and mask 
	lcdDB = (c << 2) & 0x3C; 
	lcdENPluse(); 
	// Pluse EN and wait
	Systick_Wait1ms(5); 
}

/** Clears the screen. 
*/
void lcdScreenClear(void){
	lcdWriteCommand(0x01); 
}

/** Writes data to the LCD. Bit shifts as LCD set to 4 bit
*@param The data that needs to be written 
*/
void lcdWriteData(unsigned char c){
	// Set RS to 1 to send data
	lcdRS = 0x08; 
	Systick_Wait1us(40);
	// Shift high nibble left 2 and mask 
	lcdDB = (c >> 2) & 0x3C;
	// Pluse EN and wait
	lcdENPluse(); 
	Systick_Wait1us(200); 
	// Shift low nibble right 2 and mask 
	lcdDB = (c << 2) & 0x3C; 
	// Pluse EN and wait
	lcdENPluse();
	Systick_Wait1us(200); 
	// Set RS back to 0
	lcdRS = 0x00; 
}

/** Cursor to go to DDRAM adress
*@param The DDRAM adress you want to go to
*/
void lcdGoto(unsigned char address){
	lcdWriteCommand((address | 0x80 ));
}

/** Cursor to go to DDRAM adress using row and column
*@param The DDRAM row
*@param The DDRAM column
*/
void lcdGotoRowColoum(unsigned int row, unsigned int column){
	if (row == 1){ 
		lcdGoto(0x00 | column); 
	} else if (row == 2){
		lcdGoto(0x40 | column); 
	}
}

/** Select the CGRAM address to write to
*@param Select the CGRAM address - 0 to 7
*/
void lcdSetCGRAM(unsigned char address){
	lcdWriteCommand(0x40 | (address*8)); // Times by 8 to get 0 to 7 adresses
}

/** Writes the custom character to the address in CGRAM
*@param Select the CGRAM address - 0 to 7
*@param The array that needs to be written 
*/
void lcdSetCustomCharacter(unsigned char address, unsigned char custom_char[]){
	lcdSetCGRAM(address); 
	int i = 0; 
	for(i = 0; i < 8; i++){
		lcdWriteData(custom_char[i]); // Write the array to the CGRAM
	}
	lcdWriteCommand(0x80);
}

/** Displays the custom character
*@param Select the DDRAM address - 0 to 7
*@param The row to display the character
*@param The column to display the character
*/
void lcdDisplayCustomCharacter(unsigned char address, int row, int column){
	lcdGotoRowColoum(row, column); 
	lcdWriteData(address);
}

/** Shift the display left
*/
void lcdShiftLeft(void){
	lcdWriteCommand(0x18); 
}

/** Shift the display right
*/
void lcdShiftRight(void){
	lcdWriteCommand(0x1C); 
}

/** Stop shifting
*/
void lcdShiftStop(void){
 lcdWriteCommand(0x06); 
}

/** Return the cursor to row 0 and column 0
*/
void lcdReturnHome(void){
 lcdWriteCommand(0x02); 
}

/** Writes a string to the display
*@param The string that needs to be written
*@param Shift the display left - 0 or 1
*@param Break the while loop - 0 or 1
*/
void lcdRamString(char *string, int shift, char key){
	int i = 0; 
	if (shift == 1){
		lcdShiftLeft(); // Set to shift left
	}
	while(*string){ // Do untill string is at last element 
		i++;
		if(shift == 1){
			lcdShiftStop();	// Stop the shift - slows it down
		}
		if(key == '#'){
			break; 
		}
		lcdWriteData(*string); // Write the string to the LCD
		string++; 
	}
}

/** Initialises LCD
*/
void lcdInit(void){
	// Initialise Ports A and B
	lcdPortAInit();
	lcdPortBInit(); 
	// Set EN and RS to 0
	lcdEN = 0x00;
	lcdRS = 0x00; 
	Systick_Wait1ms(15); 
	// Set DB to 0x03 and pluse
	lcdDB = 0x0C; 
	lcdENPluse(); 
	Systick_Wait1ms(5); 
	// Set DB to 0x03 and pluse
	lcdDB = 0x0C; 
	lcdENPluse(); 
	Systick_Wait1us(100); 
	// Set DB to 0x03 and pluse
	lcdDB = 0x0C;
	lcdENPluse(); 
	Systick_Wait1us(100); 
	// Set the LCD to four bit mode and pluse
	lcdDB = 0x08;
	lcdENPluse(); 
	Systick_Wait1us(100); 
	// Set display to two lines and font to 5x8
	lcdWriteCommand(0x28);
	// Set display on, set cursor on and blink on
	lcdWriteCommand(0x0F);
	// Clear display
	lcdWriteCommand(0x01);
	// Set cursor to increment and display not to shift
	lcdWriteCommand(0x06); 	
}
