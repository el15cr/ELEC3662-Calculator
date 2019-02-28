// Includes
#include "buffer.h"
#include "LCD.h"

// Global Variables
char buffer[16]; 
char result_buffer[16];
unsigned long buffer_count = 0;

static const unsigned char error_check [15][2] = {
	
		{ 0x2D, 0x2D}, // 	- and -
		{ 0x2D, 0xFD}, // 	- and /
		{ 0x2D, 0x2A}, // 	- and *
		{ 0x2D, 0x2B}, // 	- and +
		
		{ 0xFD, 0xFD}, // 	/ and /
		{ 0xFD, 0x2A}, // 	/ and *
		{ 0xFD, 0x2D}, // 	/ and -
		{ 0xFD, 0x2B}, // 	/ and +
		
		{ 0x2B, 0x2B}, // 	+ and +
		{ 0x2B, 0xFD}, // 	+ and /
		{ 0x2B, 0x2A}, // 	+ and *
		
		{ 0x2A, 0x2A}, // 	* and *
		{ 0x2A, 0xFD}, // 	* and /
		{ 0x2A, 0x2D}, // 	* and -
		{ 0x2A, 0x2B}, // 	* and +
		
};

/** Shifts the buffer array down from the start value
*@param The starting value where the shift needs to occur
*@param The value you want to relace the last shift with
*/
void bufferShift(int start, char replace){
	for (int i = start; i <= 16; i++) {
		if(buffer[i] == 0x11){
			break; 											// Break when 0x11 has been found
		} else {
			buffer[i - 1] = buffer[i]; 	// Shift the values from i - 1 
			buffer[i] = replace; 				// Replace the current value
		}
   }
	// Take buffer count down by one as the shift has occured and clear screen
	buffer_count--;
	lcdScreenClear();
}

/** Gets the lenght of buffer array based on the values present in the array
*@return The lenght of buffer array based on the values present in the array
*/
int bufferGetLenght(void){
	int array_lenght = -1; 
	int i = 0;
	for(i = 0; i <= 9; i++){
		if(buffer[i] < 0x11){	// When a value in the 0x00 to 0x09 range has been found
			array_lenght++; 		// add 1 to array_lenght
		}
	}
	return array_lenght;
}

/** Gets the lenght of buffer array based on the values present in the array
*@return The value of keep_checking - 1 or 0 
*/
int bufferCheckError(void){ // returns keep_checking which is 1 if a change has been made 0 if none has
	int keep_checking = 0; 
	int i = 0;
	int j = 0; 
	for(i = 0; i <= 16; i++){
		// If the value of buffer i and i + 1 are a sign then the caculation can not be done
		// removes the additonal signs
		for(j = 0; j <= 15; j++){
			if ((buffer[i] == error_check[0][0]) && (buffer[i + 1] == error_check[0][1])){
				buffer[i] = '+';
				bufferShift(i+2, 0x11);
				keep_checking = 1; 
				break;
			} else if ((buffer[i] == error_check[j][0]) && (buffer[i + 1] == error_check[j][1])){
				buffer[i] = buffer[i];
				bufferShift(i+2, 0x11);
				keep_checking = 1; 
				break;
			}
		}
	}
	return keep_checking; 
}

/** Writes the ascci value to the buffer array
*@param The ascii you want to write to the array
*/
void bufferWriteAscii(char ascii){
	if (ascii == '+'  || ascii == '-' || ascii == '*' || ascii == '.' || ascii == 0xFD || ascii == 0xF7){
		// Set the buffer value to either a sign or point
		buffer[buffer_count] = ascii; 
		buffer_count++; 
	} else if (ascii >= 0x30 && ascii <= 0x39){
		// or set it to a number in the 0 to 9 range (0x00 - 0x09)
		buffer[buffer_count] = ascii - 0x30; 
		buffer_count++; 
	}
}

/** Writes the ascci value to the LCD 
*@param The buffer values you want to write
*@param The row you want to write the buffer to on the LCD
*/
void bufferReadLcd(unsigned long i, int row){
	if (buffer[i] == '+' || buffer[i] == '-' || buffer[i] == '*' || buffer[i] == '.' || buffer[i] == 0xFD || buffer[i] == 0xF7){
		// read from the buffer and write the value to either a sign or point
		lcdGotoRowColoum(row,i);
		lcdWriteData(buffer[i]);
	} else if (buffer[i] >= 0x00 && buffer[i] <= 0x09){
		// or read from the buffer a ascii number in the 0 to 9 range (0x30 - 0x39)
		lcdGotoRowColoum(row,i);
		lcdWriteData((0x30 | buffer[i]));
	}
}

/** Writes to the buffer array and reads to the LCD
*@param The row you want to write the buffer to on the LCD
*@param The ascii value that is from the keypad
*/
void bufferWriteRead(int row, char key){
	// Write the ascii to the buffer
	bufferWriteAscii(key);
	// read from buffer and write data to screen
	int i;
	for(i = 0; i <= buffer_count; i += 1){
		bufferReadLcd(i, row);
	}
}

/** Writes to the results_buffer array the result and writes to the LCD
*@param The result of the calculation
*/
void bufferPrintResults(double result){
	// Uses sprintf to get a string
	sprintf(result_buffer,"= %f",result); 
	// Prints string at the row and coloum 
	lcdGotoRowColoum(2,0);
	lcdRamString(result_buffer, 0, 0);
}

/** Initialises buffer
*/
void bufferInit(void){
	// Set the array buffer to be 0x11
	memset(result_buffer, 0x00, sizeof(result_buffer));
	memset(buffer, 0x11, sizeof(buffer));
	buffer_count = 0;
}
