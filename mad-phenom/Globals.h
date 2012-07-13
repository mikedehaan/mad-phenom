#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <avr/eeprom.h>

#define MAX_PRESETS 3

// Pin Definitions
#define TRIGGER_PIN_1 5
#define TRIGGER_PIN_2 7
#define SOLENOID_PIN 6

extern uint8_t CURRENT_PRESET;

extern uint8_t EEMEM EEPROM_BALLS_PER_SECOND_1;
extern uint8_t EEMEM EEPROM_FIRING_MODE_1;
extern uint8_t EEMEM EEPROM_BURST_SIZE_1;

extern uint8_t EEMEM EEPROM_PRESET;

extern uint8_t EEMEM EEPROM_BALLS_PER_SECOND_2;
extern uint8_t EEMEM EEPROM_FIRING_MODE_2;
extern uint8_t EEMEM EEPROM_BURST_SIZE_2;

extern uint8_t EEMEM EEPROM_BALLS_PER_SECOND_3;
extern uint8_t EEMEM EEPROM_FIRING_MODE_3;
extern uint8_t EEMEM EEPROM_BURST_SIZE_3;

extern uint8_t EEPROM_BALLS_PER_SECOND[];
extern uint8_t EEPROM_FIRING_MODE[];
extern uint8_t EEPROM_BURST_SIZE[];

extern uint8_t BALLS_PER_SECOND;
extern uint8_t FIRING_MODE;
extern uint8_t BURST_SIZE;

extern uint8_t ROUND_DELAY; // delay between shots in ms
extern uint8_t PULL_DEBOUNCE;  // Debounce in ms
extern uint8_t RELEASE_DEBOUNCE;  // Debounce in ms
extern uint8_t DWELL;

#endif