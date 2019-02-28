#ifndef TEST_H
#define TEST_H

#include "keypad_test.h"
#include "buffer_test.h"
#include "math_test.h"

int runTests(){
	
	int tests_failed = 0; 
	
	// Keypad test
	if (keypadTest() >= 1){
		tests_failed++; 
	}
	
	// buffer test
	if (bufferTest() >= 1){
		tests_failed++; 
	}
	
	// math test
	if (mathTest() >= 1){
		tests_failed++; 
	}

	return tests_failed;
}

#endif
