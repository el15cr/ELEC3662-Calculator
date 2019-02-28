// Includes
#include "buffer.h" 
#include <cmath>

// Defines
#define PI 3.14159265359

// Global Variables
double numbers[10]; 
unsigned char signs[10]; 
double result = 0; 

static const unsigned char operation_order [2][2] = {
		{ 0xFD, 0x2A}, // 	/ and *
		{ 0x2B, 0x2D}, // 	- and +
};

// Function Definitions 

/** Gets the value of result
*@return The value of result
*/
double mathGetResult(void){
	return result;
}


/** Shifts the numbers and signs array down by one. 
*@param The starting value where the shift needs to occur
*@param The end of where the shift occurs 
*/
void mathShiftNumberSigns(int start, int end){
	for (int i = start; i <= end; i++) {
		numbers[i] = numbers[i + 1]; 	// Shift all values in numbers array down
		signs[i] = signs[i + 1];			// Shift all values in signs array down
   }
}

/** Gets the lenght of signs array based on the values present in the array
*@return The lenght of signs array based on the values present in the array
*/
int mathGetSignsLenght(void){
	int array_lenght = 0; 
	int i = 0;
	for(i = 0; i <= 9; i++){
		if(signs[i] > 0x00){ 	// When a value in the 0x00 to 0x09 range has been found
			array_lenght++; 		// add 1 to array_lenght
		}
	}
	return array_lenght;
}

/** Gets the lenght of numbers array based on the values present in the array
*@return The lenght of numbers array based on the values present in the array
*/
int mathGetNumbersLenght(void){
	int array_lenght = 0; 
	int i = 0;
	for(i = 0; i <= 9; i++){
		if(signs[i] > 0x00){ 	// When a value in the 0x00 to 0x09 range has been found
			array_lenght++; 		// add 1 to array_lenght
		}
	}
	return array_lenght;
}

/** Perfomrs the operation based on the input arguments
*@param The sign of the operation that needs to be performed
*@param The point at which that sign was found in the array
*@param The total lenght of signs, in terms of values in the array
*/
void mathPerformOperation(char sign, int point, int lenght){
	double temp = 0; 
	
	// Temporarily store the value of the calculation in temp 
	// Shift Numbers and Signs array down by one from where point is
	// Replace the number which has been shifted down with temp 
	if (sign == '-'){
		temp = numbers[point]-numbers[point+1]; 
		mathShiftNumberSigns(point, mathGetNumbersLenght());
		numbers[point] = temp;
	} else if(sign == '+'){
		temp = numbers[point]+numbers[point+1]; 
		mathShiftNumberSigns(point, mathGetNumbersLenght());
		numbers[point] = temp;
	} else if(sign == '*'){
		temp = numbers[point]*numbers[point+1]; 
		mathShiftNumberSigns(point, mathGetNumbersLenght());
		numbers[point] = temp; 
	} else if(sign == 0xFD){
		temp = numbers[point]/numbers[point+1]; 
		mathShiftNumberSigns(point, mathGetNumbersLenght());
		numbers[point] = temp;
	} 
}

/** Checks the order of the operations and then perfroms them 
*/
void mathPerformCalculation(void){
	// Get the number of values in the array
	int length_array = mathGetSignsLenght();
	int i = 0;
	int j = 0;
	
	for(i = 0; i <= 1; i++){ 	// Loop through operation_order
		for(j = 0; j <= 9; j++){ 	// Loop through signs
			if((operation_order[i][0] == signs[j]) || (operation_order[i][1] == signs[j])){
				// if the operation_order equals the sign 
				// then perform the operation at location j
				mathPerformOperation(signs[j], j, length_array);
				
				// Take j down by one becuase we have shifted Signs array. 
				j -= 1; 
			}
		}
	}
	
	// Set result to nubmers one as that is where the last value is stored
	result = numbers[0]; 
}

/** Checks if there is a decimal point where the number is found in buffer
*@return The number of times how many times decimal point was found and the amount of numbers after
*/
int mathCheckDecimalPoint(int start, int end, char *buffer){
	int i = 0; 
	int decimal_point = 0; 
	
	// Check from start to end where the decimal point is (if there is one)
	for(i = start; i <= end; i++){
		if(buffer[i] == '.' || decimal_point >= 1){
			decimal_point++; // When found count how many places
		}
	}
	return decimal_point;
}

/** Convets the hex numbers and converts them into a double 
*@param The start of the loop where the first value in the buffer is
*@param The end of the loop where the last value in the buffer is
*@param The buffer array
*@return The double value which has been converted from hex 
*/
double mathHexToDouble(int start, int end, char *buffer){
	int i = 0; 
	int power = end - start; // Set the power to raise the buffer value to
	double temp = 0.0;
	
	// Check if there is a decimal point and take it away from power (sets power to neg to get float value)
	power -= mathCheckDecimalPoint(start, end, buffer); 
	
	if (buffer[start] == 0xF7){ 	// if PI is found at start value return value of PI
		return PI; 
	}
	
	// Loop round from where the sign was found using start and end values
	for (i = start; i <= end; i++){ 
		if (buffer[i] == '.'){ 														// Don't do anyting if the point has been found
		} else {
			temp += ((double)buffer[i])*(pow(10.0, power)); // Cast buffer as double and times the power 100, 10, etc. 
			power--; 																				// Take the power down by one 
		}
	}
	return temp; 
}

/** Sorts the buffer and stores values into numbers and signs array
*@param The buffer array
*/
void mathSortBuffer(char *buffer){
	int i = 0; 
	int store = 0; 
	int count = 0; 
	while (bufferCheckError() == 1){} // Check for errors in the buffer array. Keep checking utill all errors have been corrected
	for (i = 0; i <= 16; i++){
		// if an operator has been found
		if(buffer[i] == '+' || buffer[i] == '-' || buffer[i] == '*' || buffer[i] == 0xFD){
			// set signs to that value - store at location store
			signs[store] = buffer[i];
			// Get number from buffer - store at location store
			// The number starts from i - count and ends at i - 1 (i - 1 becuase we found the sign at i)
			numbers[store] = mathHexToDouble((i - count), i - 1, buffer);
			// Set count to -1 becuase count gets ++ at end of the loop. So next iteration of loop
			// is 0 for count
			count = -1; 
			store++; 
		} else if ((buffer[i] != 0x11) && (buffer[i+1] == 0x11)){
			// If no more signs are found then check the value ahead of i 
			// this will always be 0x11 becuase that is what we init to 
			numbers[store] = mathHexToDouble((i - count), i, buffer); 
			break; 
		}
		count++; 
	}
}

/** Initialises math
*/
void mathInit(void){
	// Clear all of the arrays, vars and screen
	memset(numbers, 0x00, sizeof(numbers));
	memset(signs, 0x00, sizeof(signs));
	result = 0;
}
