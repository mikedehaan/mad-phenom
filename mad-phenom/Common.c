#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
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

uint8_t getPinMask(uint8_t pinNumber) {
	if (pinNumber == 2) {
		return (1 << PINB0);
	} else if (pinNumber == 3) {
		return (1 << PINB1);
	} else if (pinNumber == 4) {
		return (1 << PINB3);
	} else if (pinNumber == 5) {
		return (1 << PINB2);
	} else if (pinNumber == 6) {
		return (1 << PINA7);
	} else if (pinNumber == 7) {
		return (1 << PINA6);
	} else if (pinNumber == 8) {
		return (1 << PINA5);
	} else if (pinNumber == 9) {
		return (1 << PINA4);
	} else if (pinNumber == 10) {
		return (1 << PINA3);
	} else if (pinNumber == 11) {
		return (1 << PINA2);
	} else if (pinNumber == 12) {
		return (1 << PINA1);
	} else if (pinNumber == 13) {
		return (1 << PINA0);
	}
	
	return 0;
}

void setInputPin(uint8_t pinNumber) {
	if (pinNumber >= 2 && pinNumber <= 5) {
		DDRB &= ~(getPinMask(pinNumber));
	} else if (pinNumber >=6 && pinNumber <= 13) {
		DDRA &= ~(getPinMask(pinNumber));
	}
}

void setOutputPin(uint8_t pinNumber) {
	if (pinNumber >= 2 && pinNumber <= 5) {
		DDRB |= (getPinMask(pinNumber));
	} else if (pinNumber >= 6 && pinNumber <= 13) {
		DDRA |= (getPinMask(pinNumber));
	}
}

/*
void pinOutput(uint8_t pinNumber, uint8_t state) {
	if (pinNumber >= 2 && pinNumber <= 5) {
		if (state == HIGH) {
			PORTB |= (getPinMask(pinNumber));
		} else {
			PORTB &= ~(getPinMask(pinNumber));
		}
	} else if (pinNumber >= 6 && pinNumber <= 13) {
		if (state == HIGH) {
			PORTA |= (getPinMask(pinNumber));
		} else {
			PORTA &= ~(getPinMask(pinNumber));
		}
	}
}
*/

uint8_t pinHasInput(uint8_t pinNumber) {
	if (pinNumber >= 2 && pinNumber <= 5) {
		return (PINB & (getPinMask(pinNumber))) <= 0;
	} else if (pinNumber >= 6 && pinNumber <= 13) {
		return (PINA & (getPinMask(pinNumber))) <= 0;
	} else {
		return 0;
	}
}

void loadPreset() {
	BALLS_PER_SECOND = eeprom_read_byte(&EEPROM_BALLS_PER_SECOND[CURRENT_PRESET]);
	FIRING_MODE = eeprom_read_byte(&EEPROM_FIRING_MODE[CURRENT_PRESET]);
	BURST_SIZE = eeprom_read_byte(&EEPROM_BURST_SIZE[CURRENT_PRESET]);
	AMMO_LIMIT = eeprom_read_byte(&EEPROM_AMMO_LIMIT[CURRENT_PRESET]);
	
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
	
	DWELL = 8;
	
	// Cutting this in down since I think the tippmann default is for trigger pull and release.
	RELEASE_DEBOUNCE = 20; //Tippmann default - 52;
	PULL_DEBOUNCE = RELEASE_DEBOUNCE;
	ROUND_DELAY = (1000 - DWELL) / BALLS_PER_SECOND;
	
	// Default to full auto
	// 0 = full auto
	// 1 = three round burst
	// 2 = Auto Response
	// 3 = Semi-Auto (single shot)
	if (FIRING_MODE < 0 || FIRING_MODE > 3) {
		FIRING_MODE = 0;
	}

	// Activate the new FIRING_MODE
	trigger_changeMode();
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
	PORTA |= (1 << PINA4);
}

void solenoidOff() {
	PORTA &= ~(1 << PINA4);
}