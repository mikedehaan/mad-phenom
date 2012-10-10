/*
This file is part of mad-phenom.

mad-phenom is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

mad-phenom is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with mad-phenom.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdbool.h>
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
		3 - Semi-Auto (Single Shot)
	1 - Firing Rate (Ball Per Second)
		5 - 40
	2 - Burst size
		2 - 10

*/
#define NOT_SELECTED 255
volatile uint8_t currentMenu = 0;
volatile uint8_t menuMax = 0;
volatile uint8_t selectedMenu = NOT_SELECTED;

void getNumberFromUser(uint8_t currentNumber, uint8_t max);

void lightsOff() {
	PORTA &= ~(1 << PINA2); // GREEN
	PORTA &= ~(1 << PINA1); // RED
}

/************************************************************************/
/* Turns the orange LED on for 100ms then off                           */
/************************************************************************/
void orangeLed() {
	for (uint8_t x = 0; x < 33; x++) {
		greenOn();
		delay_ms(2);
		redOn();
		
		greenOff();
		delay_ms(1);
		redOff();
	}	
}

void presetMenu() {
	menuMax = 2;
	selectedMenu = NOT_SELECTED;
	currentMenu = 0;
	while(selectedMenu == NOT_SELECTED) {
				
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
			for (uint8_t i = 0; i < 3; i++) {
				orangeLed();
				delay_ms(100);
			}
			
			delay_ms(800);
		}
	}
	
	CURRENT_PRESET = selectedMenu;
}

void mainMenu() {
	menuMax = 4;
	selectedMenu = NOT_SELECTED;
	currentMenu = 0;
	bool state = false;
	while(selectedMenu == NOT_SELECTED) {
		if (currentMenu == 0) { // Firing Mode (toggle green then red)
			state = !state;
			
			redSet(!state);
			greenSet(state);
						
			delay_ms(100);
		} else if (currentMenu == 1) { // Firing Rate (fast green blink)
			state = !state;
			
			redOff();
			greenSet(state);
			
			delay_ms(50);
		} else if (currentMenu == 2) {  // Burst size (three red blinks)
			lightsOff();
			
			// Display as three blinks of red then pause and repeat
			for (uint8_t i = 0; i < 3; i++) {
				redOn();
				delay_ms(100);
				redOff();
				
				if (i == 2) {
					for (uint8_t i = 0; i < 100; i++) {
						delay_ms(10);
						
						if (selectedMenu != NOT_SELECTED) {
							break;
						}
					}					
				} else {
					delay_ms(100);
				}
			}
		} else if (currentMenu == 3) {  // Ammo Limit (Solid Red)
			redOn();
			greenOff();
		} else if (currentMenu == 4) {  // Safety Shot (Solid Green)
			redOff();
			greenOn();
		}			
	}
}

void firingModeMenu() {
	menuMax = 3;
	selectedMenu = NOT_SELECTED;
	currentMenu = FIRING_MODE;
	bool state = LOW;
	while(selectedMenu == NOT_SELECTED) {
		if (currentMenu == 0) {
			state = !state;
			
			redSet(state);
			greenOff();
			
			delay_ms(50);
		} else if (currentMenu == 1) { // Three Round Burst
			lightsOff();
			
			// Display as three blinks of green then pause and repeat
			for (uint8_t i = 0; i < 3; i++) {
				greenOn();
				delay_ms(100);
				greenOff();
				
				if (selectedMenu != NOT_SELECTED) {
					break;
				}
				
				if (i == 2) {
					delay_ms(1000);
				} else {
					delay_ms(100);
				}
			}				
		} else if (currentMenu == 2) { // Auto Response
			lightsOff();
	
			// Display as blink green, blink red, then pause
			greenOn();
			delay_ms(100);
			greenOff();
			delay_ms(100);
			redOn();
			delay_ms(100);
			redOff();

			if (selectedMenu != NOT_SELECTED) {
				break;
			}
		
			delay_ms(1000);
		} else if (currentMenu == 3) { // Auto Response
			redOff();
			greenOn();
		}
	}
	
	if (selectedMenu >= 0 && selectedMenu <= 3) {
		eeprom_write_byte(&EEPROM_FIRING_MODE[CURRENT_PRESET], selectedMenu);
		successBlink();
	} else {
		failureBlink();
	}
}

void ammoLimitMenu() {
	getNumberFromUser(AMMO_LIMIT, 250);
	
	// Burst size was entered into selectedMenu.  Verify it and save it.
	if (selectedMenu >= 0 && selectedMenu <= 250) {
		eeprom_write_byte(&EEPROM_AMMO_LIMIT[CURRENT_PRESET], selectedMenu);
		AMMO_LIMIT = selectedMenu;
		successBlink();
	} else {
		failureBlink();
	}
}

void safetyShotMenu() {
	getNumberFromUser(SAFETY_SHOT, 5);
	
	// Burst size was entered into selectedMenu.  Verify it and save it.
	if (selectedMenu >= 0 && selectedMenu <= 250) {
		eeprom_write_byte(&EEPROM_SAFETY_SHOT[CURRENT_PRESET], selectedMenu);
		SAFETY_SHOT = selectedMenu;
		successBlink();
	} else {
		failureBlink();
	}
}

void successBlink() {
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t x = 0; x < 200; x++) {
			greenOn();
			delay_ms(2);
			redOn();
			greenOff();
			delay_ms(1);
			redOff();
		}
	}
}

void failureBlink() {
	delay_ms(200);
	for (uint8_t i = 0; i < 10; i++) {
		redOn();
		delay_ms(50);
		redOff();
		delay_ms(50);
	}
}

void rateOfFireMenu() {
	 getNumberFromUser(BALLS_PER_SECOND, 40);
	
	// Firing rate was entered into selectedMenu.  Verify it and save it.
	if (selectedMenu >= 5 && selectedMenu <= 40) {
		eeprom_write_byte(&EEPROM_BALLS_PER_SECOND[CURRENT_PRESET], selectedMenu);
		BALLS_PER_SECOND = selectedMenu;
		successBlink();
	} else {
		failureBlink();
	}
}

void burstSizeMenu() {
	getNumberFromUser(BURST_SIZE, 10);
	
	// Burst size was entered into selectedMenu.  Verify it and save it.
	if (selectedMenu >= 2 && selectedMenu <= 10) {
		eeprom_write_byte(&EEPROM_BURST_SIZE[CURRENT_PRESET], selectedMenu);
		BURST_SIZE = selectedMenu;
		successBlink();
	} else {
		failureBlink();
	}
}

void getNumberFromUser(uint8_t currentNumber, uint8_t max) {
	bool state = false;
	menuMax = max;
	selectedMenu = NOT_SELECTED;
	currentMenu = 0;
	uint8_t currentRate = currentNumber;
	uint8_t displayRate = 0;
	delay_ms(500);
	while(selectedMenu == NOT_SELECTED) {
		if (currentMenu == 0) {
			if (state) {
				displayRate++;
			}
			
			state = !state;
			
			redOff();
			greenSet(state);
			
			delay_ms(200);
			
			if (displayRate >= currentRate) {
				displayRate = 0;
				delay_ms(1000);
			}
		} else {
			lightsOff();
		}
	}
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
		} else if (selectedMenu == 3) {
			ammoLimitMenu();
		} else if (selectedMenu == 4) {
			safetyShotMenu();
		}
	}			
}

/************************************************************************/
/* buttonHeldTime - The time in ms that the trigger was held for        */
/************************************************************************/
void configTriggerPulled(uint32_t buttonHeldTime) {
	
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
		
		greenOn();
		delay_ms(50);
		greenOff();
	}		
}