#ifndef MATHTEST_H
#define MATHTEST_H

#include "math.h"
#include "buffer.h"

int mathTest(){
	
	// initialise buffer and math
	mathInit();
	bufferInit(); 
	
	int sucess_flag = 0; 
	
	// Set values in buffer
	buffer[0] = 0x03;
	buffer[1] = '*';
	buffer[2] = 0x06;
	
	// Sort the buffer
	mathSortBuffer(buffer); 
	// Perfrom the caculation
	mathPerformCalculation();
	
	if(mathGetResult() != 18){ // if result is not 18
		sucess_flag = 1; // then it has errored
	}
	
	return sucess_flag;
}

#endif
