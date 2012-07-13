#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#define F_CPU 8000000UL

#include "Common.h"
#include "Globals.h"

/************************************************************************/
/*  COMMON ROUTINES                                                     */
/************************************************************************/

void (*fireMethod)() = 0;
void (*fireOnTriggerRelease)() = 0;

int getPinMask(int pinNumber) {
	if (pinNumber == 2) {
		return (1 << 0);
	} else if (pinNumber == 3) {
		return (1 << 1);
	} else if (pinNumber == 4) {
		return (1 << 3);
	} else if (pinNumber == 5) {
		return (1 << 2);
	} else if (pinNumber == 6) {
		return (1 << 7);
	} else if (pinNumber == 7) {
		return (1 << 6);
	} else if (pinNumber == 8) {
		return (1 << 5);
	} else if (pinNumber == 9) {
		return (1 << 4);
	} else if (pinNumber == 10) {
		return (1 << 3);
	} else if (pinNumber == 11) {
		return (1 << 2);
	} else if (pinNumber == 12) {
		return (1 << 1);
	} else if (pinNumber == 13) {
		return (1 << 0);
	}
	
	return 0;
}

void setInputPin(int pinNumber) {
	if (pinNumber >= 2 && pinNumber <= 5) {
		DDRB &= ~(getPinMask(pinNumber));
	} else if (pinNumber >=6 && pinNumber <= 13) {
		DDRA &= ~(getPinMask(pinNumber));
	}
}

void setOutputPin(int pinNumber) {
	if (pinNumber >= 2 && pinNumber <= 5) {
		DDRB |= (getPinMask(pinNumber));
	} else if (pinNumber >= 6 && pinNumber <= 13) {
		DDRA |= (getPinMask(pinNumber));
	}
}

void pinOutput(int pinNumber, int state) {
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

int pinHasInput(int pinNumber) {
	if (pinNumber >= 2 && pinNumber <= 5) {
		return (PINB & (getPinMask(pinNumber))) <= 0;
	} else if (pinNumber >= 6 && pinNumber <= 13) {
		return (PINA & (getPinMask(pinNumber))) <= 0;
	} else {
		return 0;
	}
}

void pinToggle(int pinNumber) {
	if (pinNumber >= 2 && pinNumber <= 5) {
		PORTB ^= 1 << getPinMask(pinNumber);
	} else if (pinNumber >= 6 && pinNumber <= 13) {
		PORTA ^= 1 << getPinMask(pinNumber);
	}
}

void loadPreset() {
	BALLS_PER_SECOND = eeprom_read_byte(&EEPROM_BALLS_PER_SECOND[CURRENT_PRESET]);
	FIRING_MODE = eeprom_read_byte(&EEPROM_FIRING_MODE[CURRENT_PRESET]);
	BURST_SIZE = eeprom_read_byte(&EEPROM_BURST_SIZE[CURRENT_PRESET]);
	
	// If the data is invalid, use default values
	if (BALLS_PER_SECOND < 5 || BALLS_PER_SECOND > 40) {
		BALLS_PER_SECOND = 20;
	}
	
	// Default to 3 round burst
	if (BURST_SIZE < 2 || BURST_SIZE > 10) {
		BURST_SIZE = 3;
	}
	
	DWELL = 8;
	
	// Cutting this in down since I think the tippmann default is for trigger pull and release.
	RELEASE_DEBOUNCE = 20; //Tippmann default - 52;
	ROUND_DELAY = (1000 - DWELL) / BALLS_PER_SECOND;
	
	// Default to full auto
	// 0 = full auto
	// 1 = three round burst
	if (FIRING_MODE < 0 || FIRING_MODE > 2) {
		FIRING_MODE = 0;
	}

/*
	if (FIRING_MODE == 0) { // Full Auto
		fireMethod = &fullAuto;
		fireOnTriggerRelease = 0;
		PULL_DEBOUNCE = RELEASE_DEBOUNCE;
	} else if (FIRING_MODE == 1) { // Three Round Burst
		fireMethod = &threeRoundBurst;
		fireOnTriggerRelease = 0;
		PULL_DEBOUNCE = RELEASE_DEBOUNCE;
	} else {
		PULL_DEBOUNCE = RELEASE_DEBOUNCE;
		fireMethod = &singleShot;
		fireOnTriggerRelease = &singleShot;
	}
	*/	
}

void initialize() {
	EEPROM_BALLS_PER_SECOND[0] = EEPROM_BALLS_PER_SECOND_1;
	EEPROM_FIRING_MODE[0] = EEPROM_FIRING_MODE_1;
	EEPROM_BURST_SIZE[0] = EEPROM_BURST_SIZE_1;

	EEPROM_BALLS_PER_SECOND[1] = EEPROM_BALLS_PER_SECOND_2;
	EEPROM_FIRING_MODE[1] = EEPROM_FIRING_MODE_2;
	EEPROM_BURST_SIZE[1] = EEPROM_BURST_SIZE_2;

	EEPROM_BALLS_PER_SECOND[2] = EEPROM_BALLS_PER_SECOND_3;
	EEPROM_FIRING_MODE[2] = EEPROM_FIRING_MODE_3;
	EEPROM_BURST_SIZE[2] = EEPROM_BURST_SIZE_3;

	CURRENT_PRESET = eeprom_read_byte(&EEPROM_PRESET);
	if (CURRENT_PRESET < 0 || CURRENT_PRESET > (MAX_PRESETS - 1)) {
		CURRENT_PRESET = 0;
	}

	loadPreset();
}