#include "Globals.h"
#include "Common.h"
#include "Menu.h"

/************************************************************************/
/* CONFIG MENU                                                          */
/************************************************************************/

/*

	0 - Firing Mode
		0 - Full Auto
		1 - Three Round Burst
		2 - Auto Response
	1 - Firing Rate (Ball Per Second)
		5 - 40
	2 - Burst size
		2 - 10

*/
volatile uint8_t currentMenu = 0;
volatile uint8_t menuMax = 0;
volatile uint8_t selectedMenu = -1;

/************************************************************************/
/* Turns the orange LED on for 100ms then off                           */
/************************************************************************/
void orangeLed() {
	for (uint8_t x = 0; x < 33; x++) {
		pinOutput(11, HIGH);
		delay_ms(2);
		pinOutput(11, LOW);
		pinOutput(12, HIGH);
		delay_ms(1);
		pinOutput(12, LOW);
	}	
}

uint8_t presetMenu() {
	menuMax = 2;
	selectedMenu = -1;
	currentMenu = 0;
	while(selectedMenu == -1) {
		if (currentMenu == 0) { // Preset 1
			orangeLed();
			
			delay_ms(800);
		} else if (currentMenu == 1) { // Preset 2
			for (uint8_t i = 0; i < 2; i++) {
				orangeLed();
				delay_ms(100);
			}
			
			delay_ms(800);
		} else if (currentMenu == 2) {  // Preset 3
			for (int i = 0; i < 3; i++) {
				orangeLed();
				delay_ms(100);
			}
			
			delay_ms(800);
		}
	}
	
	CURRENT_PRESET = selectedMenu;
	
	return selectedMenu;
}

uint8_t mainMenu() {
	menuMax = 2;
	selectedMenu = -1;
	currentMenu = 0;
	uint8_t state = LOW;
	while(selectedMenu == -1) {
		if (currentMenu == 0) { // Firing Mode
			if (state == HIGH) {
				state = LOW;
			} else {
				state = HIGH;
			}
			
			pinOutput(12, !state);
			pinOutput(11, state);
			delay_ms(100);
		} else if (currentMenu == 1) { // Firing Rate
			if (state == HIGH) {
				state = LOW;
			} else {
				state = HIGH;
			}
			
			pinOutput(12, LOW);
			pinOutput(11, state);
			delay_ms(50);
		} else if (currentMenu == 2) {  // Burst size
			pinOutput(12, LOW);
			pinOutput(11, LOW);
			
			// Display as three blinks of red then pause and repeat
			for (uint8_t i = 0; i < 3; i++) {
				pinOutput(12, HIGH);
				delay_ms(100);
				pinOutput(12, LOW);
				
				if (i == 2) {
					for (uint8_t i = 0; i < 100; i++) {
						delay_ms(10);
						
						if (selectedMenu > -1) {
							break;
						}
					}					
				} else {
					delay_ms(100);
				}
			}
		}
	}
	
	return selectedMenu;
}

uint8_t firingModeMenu() {
	menuMax = 2;
	selectedMenu = -1;
	currentMenu = FIRING_MODE;
	uint8_t state = LOW;
	while(selectedMenu == -1) {
		if (currentMenu == 0) {
			if (state == HIGH) {
				state = LOW;
			} else {
				state = HIGH;
			}
			
			pinOutput(11, LOW);
			pinOutput(12, state);
			delay_ms(50);
		} else if (currentMenu == 1) { // Three Round Burst
			pinOutput(12, LOW);
			pinOutput(11, LOW);
			
			// Display as three blinks of green then pause and repeat
			for (uint8_t i = 0; i < 3; i++) {
				pinOutput(11, HIGH);
				delay_ms(100);
				pinOutput(11, LOW);
				
				if (selectedMenu > -1) {
					break;
				}
				
				if (i == 2) {
					delay_ms(1000);
				} else {
					delay_ms(100);
				}
			}				
		} else if (currentMenu == 2) { // Auto Response
			pinOutput(12, LOW);
			pinOutput(11, LOW);
	
			// Display as blink green, blink red, then pause
			pinOutput(11, HIGH);
			delay_ms(100);
			pinOutput(11, LOW);
			delay_ms(100);
			pinOutput(12, HIGH);
			delay_ms(100);
			pinOutput(12, LOW);
		
			if (selectedMenu > -1) {
				break;
			}
		
			delay_ms(1000);
		}
	}
	
	if (selectedMenu >= 0 && selectedMenu <= 2) {
		eeprom_write_byte(&EEPROM_FIRING_MODE[CURRENT_PRESET], selectedMenu);
		successBlink();
	} else {
		failureBlink();
	}
	
	return selectedMenu;	
}

void successBlink() {
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t x = 0; x < 200; x++) {
			pinOutput(11, HIGH);
			delay_ms(2);
			pinOutput(11, LOW);
			pinOutput(12, HIGH);
			delay_ms(1);
			pinOutput(12, LOW);
		}
	}
}

void failureBlink() {
	delay_ms(200);
	for (uint8_t i = 0; i < 10; i++) {
		pinOutput(12, HIGH);
		delay_ms(50);
		pinOutput(12, LOW);
		delay_ms(50);
	}
}

uint8_t rateOfFireMenu() {
	uint8_t state = LOW;
	menuMax = 40;
	selectedMenu = -1;
	currentMenu = 0;
	uint8_t currentRate = BALLS_PER_SECOND;
	uint8_t displayRate = 0;
	delay_ms(500);
	while(selectedMenu == -1) {
		if (currentMenu == 0) {
			if (state == HIGH) {
				displayRate++;
				state = LOW;
			} else {
				state = HIGH;
			}
			
			pinOutput(11, state);
			pinOutput(12, LOW);
			delay_ms(200);
		
			if (displayRate >= currentRate) {
				displayRate = 0;
				delay_ms(1000);
			}
		} else {
			pinOutput(11, LOW);
			pinOutput(12, LOW);
		}
	}
	
	// Firing rate was entered into selectedMenu.  Verify it and save it.
	
	if (selectedMenu >= 5 && selectedMenu <= 40) {
		eeprom_write_byte(&EEPROM_BALLS_PER_SECOND[CURRENT_PRESET], selectedMenu);
		BALLS_PER_SECOND = selectedMenu;
		successBlink();
	} else {
		failureBlink();
	}
	
	return selectedMenu;
}

uint8_t burstSizeMenu() {
	uint8_t state = LOW;
	menuMax = 10;
	selectedMenu = -1;
	currentMenu = 0;
	uint8_t currentRate = BURST_SIZE;
	uint8_t displayRate = 0;
	delay_ms(500);
	while(selectedMenu == -1) {
		if (currentMenu == 0) {
			if (state == HIGH) {
				displayRate++;
				state = LOW;
			} else {
				state = HIGH;
			}
			
			pinOutput(11, state);
			pinOutput(12, LOW);
			delay_ms(200);
			
			if (displayRate >= currentRate) {
				displayRate = 0;
				delay_ms(1000);
			}
		} else {
			pinOutput(11, LOW);
			pinOutput(12, LOW);
		}
	}
	
	// Burst size was entered into selectedMenu.  Verify it and save it.
	if (selectedMenu >= 2 && selectedMenu <= 10) {
		eeprom_write_byte(&EEPROM_BURST_SIZE[CURRENT_PRESET], selectedMenu);
		BURST_SIZE = selectedMenu;
		successBlink();
	} else {
		failureBlink();
	}
	
	return selectedMenu;
}

void handleConfig() {
	while (1) {
		presetMenu();
		
		loadPreset();
		
		mainMenu();
	
		if (selectedMenu == 0) {
			firingModeMenu();
		} else if (selectedMenu == 1) {
			rateOfFireMenu();
		} else if (selectedMenu == 2) {
			burstSizeMenu();
		}
	}			
}

/************************************************************************/
/* buttonHeldTime - The time in ms that the trigger was held for        */
/************************************************************************/
void configTriggerPulled(uint8_t buttonHeldTime) {
	
	// Select the menu
	if (buttonHeldTime >= 1000) {
		selectedMenu = currentMenu;
	} else {
		// Cycle through the menus
		if (currentMenu == menuMax) {
			currentMenu = 0;
		} else {
			currentMenu++;
		}
		
		pinOutput(11, HIGH);
		delay_ms(50);
		pinOutput(11, LOW);
	}		
}