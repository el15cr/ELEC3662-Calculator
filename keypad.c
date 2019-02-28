// Port D defines
#define GPIO_PORTD_DATA_R  	(*((volatile unsigned long *) 0x400073FC))
#define GPIO_PORTD_DIR_R   	(*((volatile unsigned long *) 0x40007400))
#define GPIO_PORTD_AFSEL_R  (*((volatile unsigned long *) 0x40007420))
#define GPIO_PORTD_PUR_R  	(*((volatile unsigned long *) 0x40007510))
#define GPIO_PORTD_ODR_R		(*((volatile unsigned long *) 0x4000750C))
#define GPIO_PORTD_DEN_R   	(*((volatile unsigned long *) 0x4000751C))
#define GPIO_PORTD_AMSEL_R  (*((volatile unsigned long *) 0x40007528))
#define GPIO_PORTD_PCTL_R   (*((volatile unsigned long *) 0x4000752C))
// Port E defines
#define GPIO_PORTE_DATA_R  	(*((volatile unsigned long *) 0x400243FC))
#define GPIO_PORTE_DIR_R   	(*((volatile unsigned long *) 0x40024400))
#define GPIO_PORTE_AFSEL_R  (*((volatile unsigned long *) 0x40024420))
#define GPIO_PORTE_PUR_R  	(*((volatile unsigned long *) 0x40024510))
#define GPIO_PORTE_PDR_R		(*((volatile unsigned long *) 0x40024514))
#define GPIO_PORTE_ODR_R		(*((volatile unsigned long *) 0x4002450C))
#define GPIO_PORTE_DEN_R   	(*((volatile unsigned long *) 0x4002451C))
#define GPIO_PORTE_AMSEL_R  (*((volatile unsigned long *) 0x40024528))
#define GPIO_PORTE_PCTL_R   (*((volatile unsigned long *) 0x4002452C))
#define GPIO_PORTE_IS_R 		(*((volatile unsigned long *) 0x40024404))
#define GPIO_PORTE_IBE_R 		(*((volatile unsigned long *) 0x40024408))
#define GPIO_PORTE_IEV_R  	(*((volatile unsigned long *) 0x4002440C))
#define GPIO_PORTE_ICR_R 		(*((volatile unsigned long *) 0x4002441C))
#define GPIO_PORTE_IM_R 		(*((volatile unsigned long *) 0x40024410))
// Misc defines
#define SYSCTL_RCGC2_R      (*((volatile unsigned long *) 0x400FE108))
#define NVIC_PRI1_R 				(*((volatile unsigned long *) 0xE000E404))
#define NVIC_EN0_R 					(*((volatile unsigned long *) 0xE000E100))	
// Bit Specific Ports 
#define keypadIn 						(*((volatile unsigned long *) 0x4002403C)) // keypadIn - PE0 - PE3
#define keypadOut 					(*((volatile unsigned long *) 0x4000703C)) // keypadOut - PD0 - PD3
// Includes
#include "PLL.h"

// Global Variables
static const unsigned char key_decode [20][2] = {
		{ 0x11, '1' }, // Col 1 - Row 1
		{ 0x12, '4' }, // Col 1 - Row 2
		{ 0x14, '7' }, // Col 1 - Row 3
		{ 0x18, '.' }, // Col 1 - Row 4
		{ 0x21, '2' }, // Col 2 - Row 1
		{ 0x22, '5' }, // Col 2 - Row 2
		{ 0x24, '8' }, // Col 2 - Row 3
		{ 0x28, '0' }, // Col 2 - Row 4
		{ 0x31, '3' }, // Col 3 - Row 1
		{ 0x32, '6' }, // Col 3 - Row 2
		{ 0x34, '9' }, // Col 3 - Row 3
		{ 0x38, '#' }, // Col 3 - Row 4
		{ 0x41, '+' }, // Col 4 - Row 1
		{ 0x42, '-' }, // Col 4 - Row 2
		{ 0x44, '*' }, // Col 4 - Row 3
		{ 0x48, 0xFD }, // Col 4 - Row 4
		{ 0x40, 0x40 }, // Reset command
		{ 0x50, 0x50 }, // Clear command
		{ 0x59, 0x59 }, // Clear command
		{ 0x60, 0xF7 }, // pi
};

volatile unsigned long key_press_flag = 0;
char key_press = 0x00; 
 
// Functions Definitions 

/** Initialises Port D
*/
void keypadPortDInit(void){ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000008;     // 1) Port D clock
  delay = SYSCTL_RCGC2_R;           // delay         
  GPIO_PORTD_AMSEL_R &= 0x00;       // 2) disable analog function
  GPIO_PORTD_PCTL_R &= 0x00000000;  // 3) GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R |= 0x0F;         // 4) PD0 - PD3 output  
  GPIO_PORTD_AFSEL_R &= 0x00;       // 5) no alternate function   
	GPIO_PORTD_PUR_R |= ~0x0F;        // Disable pullup resistors on PD0 - PD3  
  GPIO_PORTD_DEN_R |= 0x0F;         // 6) enable digital pins PD0-PD3  
	keypadOut |= 0x0F; 								// Set port D data
}

/** Initialises Port E
*/
void keypadPortEInit(void){ 
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000010;     // 1) Port E clock
  delay = SYSCTL_RCGC2_R;           // delay        
  GPIO_PORTE_AMSEL_R &= 0x00;       // 2) disable analog function
  GPIO_PORTE_PCTL_R &= 0x00000000;  // 3) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R &= ~0x0F;        // 4) PE0 - PE3 input, 
  GPIO_PORTE_AFSEL_R &= 0x00;       // 5) no alternate function
	GPIO_PORTE_PDR_R |= 0x0F;         // enable pulldown resistors on PE0 - PE3  
	GPIO_PORTE_ODR_R |= 0x0F;         // enable open drain on PE0 - PE3  	
  GPIO_PORTE_DEN_R |= 0x0F;         // 6) enable digital pins PE0 - PE3 
	GPIO_PORTE_IS_R &= ~0x0F;     		// PE0 - 3 are edge-sensitive
  GPIO_PORTE_IBE_R &= ~0x0F;    		// PE0 - 3 are not both edges
  GPIO_PORTE_IEV_R |= 0x0F;    			// PE0 - 3 rising edge event
  GPIO_PORTE_ICR_R = 0x0F;      		// clear flags PE0 - 3
  GPIO_PORTE_IM_R |= 0x0F;      		// arm interrupt on PE0 - 3
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFF0F)|0x000000A0; // priority 5
  NVIC_EN0_R = 0x00000010;      		// enable interrupt 5 in NVIC
	keypadIn |= 0x00; 								// Set port E data
	key_press_flag = 0; 
}

/** Checks the row for any given column i
*@return 1 or 0. Depends on if the value of Port E data has changed
*/
int keypadCheckRow(void){
	// Masks the value (of port E data) and checks if the masked value does not equal the value of 0x00
	// If it does not something has changed. 
	if((keypadIn&0x0F) != 0x00){
		return 1; 
	} else {
		return 0; 
	}
}

/** Sets the column active high based on the input value i
*@param The column that needs to be set high 
*/
void keypadSetColumn(int i){
	// Sets the column value active high (positive logic) based on input value i. 
	// 0001(0x01) - 0010(0x02) - 0100(0x04) - 1000(0x08)
	keypadOut = (1 << (i - 1));
}

/** Varies the output of Port D (column) and checks the row. 
*@return The column and row that the key press was detected. High nibble is column and low is the row. 
*/
unsigned char keypadRead(void){
	unsigned char keypad_in = 0x00; 
	int i = 0; 
	for(i = 1; i <= 4; i++){
		keypadSetColumn(i); // Vary the value of column and sets the column value based on the loop. 1 to 4. 
		if(keypadCheckRow() == 1){ 
			// Shift the column value into high nibble
			keypad_in += (i << 4) | (keypadIn & 0x0F);
			//Port E data and masks to low nibble. Or results of both
		}
	}
	return keypad_in; 
}

/** Finds the ascii value for the key that was pressed. 
*@param The key that was pressed. High nibble is column and low is the row. 
*@return The ascii charater that the keypad press is associated with 
*/
char keypadDecodeKeyPress(unsigned char key_press_found){
	char ascii = 0x00; 
	int i = 0; 
	for(i = 0; i <= 20; i++){
		// Loops through all 20 elements of the array and checks if input argument matchs the first element (the result of readKeypad)
		if (key_decode[i][0] == key_press_found){
			ascii = key_decode[i][1];  //If it does set the returning value (of decodeKeyPress) to the second element of the array (string literal)
			break;
		} else if (i == 20){
			ascii = 0x00; // If no result has been found set to 0x00. 
		}
	}
	keypadOut = 0xFF; // Set keypadOut to all be on
	return ascii; 
}

/** Checks if the ISR flag has been set and then perfroms the action
*/
void keypadCheckFlag(void){
	if (key_press_flag == 1){
		key_press = keypadDecodeKeyPress(keypadRead()); // Read the keypad
		key_press_flag = 0; 														// Set the flag back to 0
	} 
}	

/** Checks if the ISR flag has been set and then perfroms the action
*@return The ascii from the key press that has been found
*/
char keypadGetKeyPress(void){
	return key_press; 
}

/** Set the value of key_press back to 0x00
*/
void keypadSetKeyPress(void){
	key_press = 0x00; 
}

/** The ISR handler from the NVIC
*/
void keypadHandler(void){
  GPIO_PORTE_ICR_R = 0x0F; 	// acknowledge flags and clear
	Systick_Wait1ms(10); 			// Debouncing - wait for 10 ms 
	key_press_flag = 1; 			// Set the ISR flag to 1
}

/** Initialises every that is need for keypad
*/
void keypadInit(void){
	keypadPortDInit();
	keypadPortEInit(); 
}
