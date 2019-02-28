#ifndef BUFFERTEST_H
#define BUFFERTEST_H

#include "buffer.h"

int bufferTest(){
	
	// initialise buffer
	bufferInit(); 
	
	int sucess_flag = 0; 
	
	// Write a * to buffer
	bufferWriteAscii('*');
	
	if(buffer[0] != '*'){ // if it does not = * 
		sucess_flag = 1; // then error
	}
	
	return sucess_flag;
}

#endif
