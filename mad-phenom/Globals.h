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
#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <avr/eeprom.h>

#define MAX_PRESETS 3

#define TRIGGER_PIN_1 5
#define TRIGGER_PIN_2 7
#define PIN_LED_GREEN 11
#define PIN_LED_RED 12
#define PIN_LED_YELLOW 6
#define PIN_SOLENOID 6
#define PIN_PUSHBUTTON 3

#define PULL_DEBOUNCE 20



extern uint8_t EEMEM EEPROM_BALLS_PER_SECOND_1;
extern uint8_t EEMEM EEPROM_FIRING_MODE_1;
extern uint8_t EEMEM EEPROM_BURST_SIZE_1;
extern uint8_t EEMEM EEPROM_AMMO_LIMIT_1;
extern uint8_t EEMEM EEPROM_SAFETY_SHOT_1;

extern uint8_t EEMEM EEPROM_BALLS_PER_SECOND_2;
extern uint8_t EEMEM EEPROM_FIRING_MODE_2;
extern uint8_t EEMEM EEPROM_BURST_SIZE_2;
extern uint8_t EEMEM EEPROM_AMMO_LIMIT_2;
extern uint8_t EEMEM EEPROM_SAFETY_SHOT_2;

extern uint8_t EEMEM EEPROM_BALLS_PER_SECOND_3;
extern uint8_t EEMEM EEPROM_FIRING_MODE_3;
extern uint8_t EEMEM EEPROM_BURST_SIZE_3;
extern uint8_t EEMEM EEPROM_AMMO_LIMIT_3;
extern uint8_t EEMEM EEPROM_SAFETY_SHOT_3;

#ifdef X7CLASSIC
extern uint8_t CURRENT_PRESET[2];
extern uint8_t EEMEM EEPROM_PRESET_1;
extern uint8_t EEMEM EEPROM_PRESET_2;

extern uint8_t EEMEM EEPROM_BALLS_PER_SECOND_4;
extern uint8_t EEMEM EEPROM_FIRING_MODE_4;
extern uint8_t EEMEM EEPROM_BURST_SIZE_4;
extern uint8_t EEMEM EEPROM_AMMO_LIMIT_4;
extern uint8_t EEMEM EEPROM_SAFETY_SHOT_4;

extern uint8_t EEMEM EEPROM_BALLS_PER_SECOND_5;
extern uint8_t EEMEM EEPROM_FIRING_MODE_5;
extern uint8_t EEMEM EEPROM_BURST_SIZE_5;
extern uint8_t EEMEM EEPROM_AMMO_LIMIT_5;
extern uint8_t EEMEM EEPROM_SAFETY_SHOT_5;

extern uint8_t EEMEM EEPROM_BALLS_PER_SECOND_6;
extern uint8_t EEMEM EEPROM_FIRING_MODE_6;
extern uint8_t EEMEM EEPROM_BURST_SIZE_6;
extern uint8_t EEMEM EEPROM_AMMO_LIMIT_6;
extern uint8_t EEMEM EEPROM_SAFETY_SHOT_6;

extern uint8_t EEPROM_BALLS_PER_SECOND[2][MAX_PRESETS];
extern uint8_t EEPROM_FIRING_MODE[2][MAX_PRESETS];
extern uint8_t EEPROM_BURST_SIZE[2][MAX_PRESETS];
extern uint8_t EEPROM_AMMO_LIMIT[2][MAX_PRESETS];
extern uint8_t EEPROM_SAFETY_SHOT[2][MAX_PRESETS];

extern uint8_t currentSelector;
#else
extern uint8_t CURRENT_PRESET;
extern uint8_t EEMEM EEPROM_PRESET;

extern uint8_t EEPROM_BALLS_PER_SECOND[];
extern uint8_t EEPROM_FIRING_MODE[];
extern uint8_t EEPROM_BURST_SIZE[];
extern uint8_t EEPROM_AMMO_LIMIT[];
extern uint8_t EEPROM_SAFETY_SHOT[];
#endif

extern uint8_t BALLS_PER_SECOND;
extern uint8_t FIRING_MODE;
extern uint8_t BURST_SIZE;
extern uint8_t AMMO_LIMIT;
extern uint8_t SAFETY_SHOT;

extern uint8_t ROUND_DELAY; // delay between shots in ms
extern uint8_t RELEASE_DEBOUNCE;  // Debounce in ms
extern uint8_t DWELL;

extern uint8_t shotsFired;
#endif