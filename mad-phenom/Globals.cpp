#include <avr/eeprom.h>
#include "Globals.h"

uint8_t CURRENT_PRESET = 0;

uint8_t EEMEM EEPROM_BALLS_PER_SECOND_1;
uint8_t EEMEM EEPROM_FIRING_MODE_1;
uint8_t EEMEM EEPROM_BURST_SIZE_1;

uint8_t EEMEM EEPROM_PRESET;

uint8_t EEMEM EEPROM_BALLS_PER_SECOND_2;
uint8_t EEMEM EEPROM_FIRING_MODE_2;
uint8_t EEMEM EEPROM_BURST_SIZE_2;

uint8_t EEMEM EEPROM_BALLS_PER_SECOND_3;
uint8_t EEMEM EEPROM_FIRING_MODE_3;
uint8_t EEMEM EEPROM_BURST_SIZE_3;

uint8_t EEPROM_BALLS_PER_SECOND[MAX_PRESETS];
uint8_t EEPROM_FIRING_MODE[MAX_PRESETS];
uint8_t EEPROM_BURST_SIZE[MAX_PRESETS];

uint8_t BALLS_PER_SECOND;
uint8_t FIRING_MODE;
uint8_t BURST_SIZE;

uint8_t ROUND_DELAY; // delay between shots in ms
uint8_t PULL_DEBOUNCE;  // Debounce in ms
uint8_t RELEASE_DEBOUNCE;  // Debounce in ms
uint8_t DWELL;


uint32_t millis(const Clock &clock) {
	uint32_t result = 0;
	result += clock.millisecond;
	result += clock.second * 1000L;
	result += clock.minute * 60L * 1000L;
	result += clock.hour * 60L * 60L * 1000L;
	return result;
}

/*
  Initialize the EEPROM array
*/


