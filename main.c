// Documentation Section: _
// main.c
// Authors: Callum Rosedale	
// Date: 28/02/19

// Defines
#define WITH_TESTING 0

// Pre-processor Directives
#include "keypad.h"
#include "LCD.h"
#include "PLL.h"
#include "math.h"
#include "eeprom.h"
#include "buffer.h" 
#if WITH_TESTING
	#include "test.h"
#endif

enum state{ENTER_PASSWORD, CHANGE_PASSWORD, SET_PASSWORD, WELCOME, CALCULATOR}; 

// Global Variables
char key = 0x00; 
unsigned long password_store; 
enum state calcuator_state = ENTER_PASSWORD; 
unsigned long test_failed = 0; 

// Function Prototypes
void menu(void);
void password(void); 
void passwordChange(void); 
void setNewPassword(void);
void welcome(void);
void calculator(void);
void checkKeypad(void);
void clearAll(void);

// MAIN: Mandatory for a C Program to be executable
int main(void){
	
	// Run tests if WITH_TESTING == 1
	#if WITH_TESTING
		int number_failed_tests = runTests();
		if (number_failed_tests >= 1){
			test_failed = 1; 		
		}
	#endif
		
	// Initialisers 
 	PLLInit();	
	eepromInit();
	keypadInit(); 
	lcdInit();
	bufferInit();
	mathInit(); 
	password_store = eepromGetPassword(); // Get password from EEPROM
	
  while(1){
		checkKeypad(); 	// Check keypad press and ISR
		menu(); 				// Load the menu
  }
}

void menu(void){
	switch(calcuator_state){
		case ENTER_PASSWORD:
			password(); 
			break;
		case CHANGE_PASSWORD: 
			passwordChange();
			break;
		case SET_PASSWORD: 
			setNewPassword(); 
			break;
		case WELCOME: 
			welcome(); 
			break;
		case CALCULATOR: 
			calculator();
			break;
		default:
			password();
			break; 
	}
}

void password(){
	lcdGotoRowColoum(1,0); // Write string to the first row
	lcdRamString("Enter password: ", 0, 0); 
	bufferWriteRead(2, key); // Show password being entered to second row
	
	// cast and set the value of temp_password
	unsigned long temp_password = ((unsigned long)mathHexToDouble(0, bufferGetLenght(), buffer));
	
	if (key == '#' && (temp_password  == password_store)){ // If pass is true then go to next state
		clearAll();
		calcuator_state = CHANGE_PASSWORD; 
	} else if (key == '#' && (temp_password  != password_store)){ // Else password is wrong
		clearAll();
		lcdRamString("Wrong password!!!", 0, 0); 
		Systick_Wait1ms(2000);
		clearAll();
	}
}

void passwordChange(){
	lcdGotoRowColoum(1,0); // Write string to the first row
	lcdRamString("Change password ", 0, 0);
	lcdGotoRowColoum(2,0); // Write string to the second row
	lcdRamString("1 or 0", 0, 0);
	if (key == '1'){ // Change the password if 1 
		clearAll();
		calcuator_state = SET_PASSWORD; 
	} else if (key == '0'){ // Go to next state if 0
		clearAll();
		calcuator_state = WELCOME; 
	}
}

void setNewPassword(void){
	lcdGotoRowColoum(1,0); // Write string to the first row
	lcdRamString("Enter new pass ", 0, 0);
	unsigned long temp_password  = ((unsigned long)mathHexToDouble(0, bufferGetLenght(), buffer));
	bufferWriteRead(2, key);
	if(key == '#'){
		eepromSetPassowrd(temp_password); // Set the new password to temp
		password_store = eepromGetPassword(); // Set password store as the new passowrd from EEPROM
		clearAll();
		calcuator_state = ENTER_PASSWORD; // Go back to enter the new password
	}
}

void welcome(void){
	// Display welcome message and infrom user about controls 
	// Press # to go to calculator
	lcdGotoRowColoum(1,0);
	lcdRamString(" Press # to continue. ", 1, key); 
	lcdRamString(" A = + B = - C = *", 1, key);
	lcdGotoRowColoum(2,0);	
	lcdRamString("D = / Command to clear * and # ", 1, key); 
	
	if (key == '#'){
		clearAll();	
		calcuator_state = CALCULATOR; // Next state calculator
	}
}

void calculator(void){
	
	bufferWriteRead(1, key); // Write the numbers to the LCD 
	
	// If hash is pressed do calculation 
	if(key == '#'){
		mathSortBuffer(buffer); 
		mathPerformCalculation();
	}
	
	// If star and hash are pressed clear all
	if (key == 0x50){
		clearAll(); 
	}
	
	// If star and 0 are pressed go to enter password
	if (key == 0x40){
		clearAll();
		calcuator_state = ENTER_PASSWORD;  
	}
	
	bufferPrintResults(mathGetResult()); // Print the result to the LCD
}

void checkKeypad(void){
	keypadCheckFlag(); 					// Check the ISR flag and perform the operation if true
	key = keypadGetKeyPress();	// Set the golabl var key to the ascii value from keypad
	keypadSetKeyPress(); 				// Reset the value from keypad back to zero
}

void clearAll(void){
	// reset all the values 
	bufferInit();
	mathInit(); 
	lcdScreenClear(); 
}
