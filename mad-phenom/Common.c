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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "Common.h"
#include "Globals.h"
#include "Trigger.h"

/************************************************************************/
/*  COMMON ROUTINES                                                     */
/************************************************************************/

void delay_ms(uint16_t ms){
	for (uint16_t i = 0; i < ms; i++) {
		_delay_ms(1);
	}
}

void loadPreset() {

	BALLS_PER_SECOND = eeprom_read_byte(&EEPROM_BALLS_PER_SECOND[CURRENT_PRESET]);
	FIRING_MODE = eeprom_read_byte(&EEPROM_FIRING_MODE[CURRENT_PRESET]);
	BURST_SIZE = eeprom_read_byte(&EEPROM_BURST_SIZE[CURRENT_PRESET]);
	AMMO_LIMIT = eeprom_read_byte(&EEPROM_AMMO_LIMIT[CURRENT_PRESET]);
	SAFETY_SHOT = eeprom_read_byte(&EEPROM_SAFETY_SHOT[CURRENT_PRESET]);

	// If the data is invalid, use default values
	if (BALLS_PER_SECOND < 5 || BALLS_PER_SECOND > 40) {
		BALLS_PER_SECOND = 20;
	}
	
	// Default to 3 round burst
	if (BURST_SIZE < 2 || BURST_SIZE > 10) {
		BURST_SIZE = 3;
	}
	
	if (AMMO_LIMIT > 250) {
		AMMO_LIMIT = 0;
	}
	
	if (SAFETY_SHOT < 0 || SAFETY_SHOT > 5) {
		SAFETY_SHOT = 0;
	}

#ifdef DWELL_DEBOUNCE

	USER_DWELL = eeprom_read_byte(&EEPROM_USER_DWELL);
	USER_DEBOUNCE = eeprom_read_byte(&EEPROM_USER_DEBOUNCE);

	if (USER_DWELL < 2 || USER_DWELL > 20) {
		USER_DWELL = 8;
	}

	if (USER_DEBOUNCE < 5 || USER_DEBOUNCE > 50) {
		USER_DEBOUNCE = 20;
	}

	RELEASE_DEBOUNCE = USER_DEBOUNCE;
	DWELL = USER_DWELL;

#else
	DWELL = 8;

	// Cutting this in down since I think the tippmann default is for trigger pull and release.
	RELEASE_DEBOUNCE = 20; //Tippmann default - 52;
#endif

#ifdef X7CLASSIC
	// If the selector switch is on "F" force semi-auto
	if (currentSelector == 0) {
		BALLS_PER_SECOND = 20;
		FIRING_MODE = 3;
		BURST_SIZE = 1;
		AMMO_LIMIT = 0;
		SAFETY_SHOT = 0;
	}
#endif

	ROUND_DELAY = (1000 / BALLS_PER_SECOND) - DWELL;

	// Default to full auto
	// 0 = full auto
	// 1 = three round burst
	// 2 = Auto Response
	// 3 = Semi-Auto (single shot)
	if (FIRING_MODE < 0 || FIRING_MODE > 3) {
		FIRING_MODE = 0;
	}

	// Activate the new FIRING_MODE
	// This was causing serious issues with X7CLASSIC and the F/FA switch
	// Basically, "F" was always firing as if it were full-auto
	// The F/FA switch constantly toggles between F/FA when set to "F" (sub-microsecond fast)
	//trigger_pulled = false;
}

void initialize() {
	EEPROM_BALLS_PER_SECOND[0] = EEPROM_BALLS_PER_SECOND_1;
	EEPROM_FIRING_MODE[0] = EEPROM_FIRING_MODE_1;
	EEPROM_BURST_SIZE[0] = EEPROM_BURST_SIZE_1;
	EEPROM_AMMO_LIMIT[0] = EEPROM_AMMO_LIMIT_1;

	EEPROM_BALLS_PER_SECOND[1] = EEPROM_BALLS_PER_SECOND_2;
	EEPROM_FIRING_MODE[1] = EEPROM_FIRING_MODE_2;
	EEPROM_BURST_SIZE[1] = EEPROM_BURST_SIZE_2;
	EEPROM_AMMO_LIMIT[1] = EEPROM_AMMO_LIMIT_2;

	EEPROM_BALLS_PER_SECOND[2] = EEPROM_BALLS_PER_SECOND_3;
	EEPROM_FIRING_MODE[2] = EEPROM_FIRING_MODE_3;
	EEPROM_BURST_SIZE[2] = EEPROM_BURST_SIZE_3;
	EEPROM_AMMO_LIMIT[2] = EEPROM_AMMO_LIMIT_3;

	CURRENT_PRESET = eeprom_read_byte(&EEPROM_PRESET);
	if (CURRENT_PRESET < 0 || CURRENT_PRESET > (MAX_PRESETS - 1)) {
		CURRENT_PRESET = 0;
	}

	loadPreset();
}

void togglePreset(){
	if (CURRENT_PRESET >= (MAX_PRESETS - 1)) {
		CURRENT_PRESET = 0;
	} else {
		CURRENT_PRESET++;
	}
	
	eeprom_write_byte(&EEPROM_PRESET, CURRENT_PRESET);
	
	loadPreset();
}

void redOff() {
	PORTA &= ~(1 << PINA1); // RED
}

void greenOff() {
	PORTA &= ~(1 << PINA2); // GREEN
}

void redOn() {
	PORTA |= (1 << PINA1); // RED
}

void greenOn() {
	PORTA |= (1 << PINA2); // RED
}

void redSet(bool state) {
	if (state) {
		redOn();
	} else {
		redOff();
	}
}

void greenSet(bool state) {
	if (state) {
		greenOn();
	} else {
		greenOff();
	}
}

void solenoidOn() {
	PORTA |= (1 << PINA7);
}

void solenoidOff() {
	PORTA &= ~(1 << PINA7);
}

bool pushButtonHasInput() {
	return (PINB & (1 << PINB1)) <= 0;
}