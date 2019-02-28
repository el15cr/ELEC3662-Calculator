#ifndef KEYPADTEST_H
#define KEYPADTEST_H

#include "keypad.h"

int keypadTest(){
	
	// initialise keypad
	keypadInit(); 
	
	int sucess_flag = 0; 
	
	// Read from keypad and decode 
	if (keypadDecodeKeyPress(keypadRead()) != 0x00){ // if not 0x00 then something is wrong
		sucess_flag = 1; 
	}
	
	// set a value to decode
	if (keypadDecodeKeyPress(0x11) != '1'){ // if it does not equal 1
		sucess_flag = 1; // then error
	}
	
	// check the row 
	if(keypadCheckRow() != 0){
		sucess_flag = 1; 
	}
	
	return sucess_flag;
}

#endif
