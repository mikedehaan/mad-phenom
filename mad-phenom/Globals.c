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
#include <avr/eeprom.h>
#include "Globals.h"

uint8_t CURRENT_PRESET = 0;

uint8_t EEMEM EEPROM_BALLS_PER_SECOND_1;
uint8_t EEMEM EEPROM_FIRING_MODE_1;
uint8_t EEMEM EEPROM_BURST_SIZE_1;
uint8_t EEMEM EEPROM_AMMO_LIMIT_1;
uint8_t EEMEM EEPROM_SAFETY_SHOT_1;

uint8_t EEMEM EEPROM_PRESET;

uint8_t EEMEM EEPROM_BALLS_PER_SECOND_2;
uint8_t EEMEM EEPROM_FIRING_MODE_2;
uint8_t EEMEM EEPROM_BURST_SIZE_2;
uint8_t EEMEM EEPROM_AMMO_LIMIT_2;
uint8_t EEMEM EEPROM_SAFETY_SHOT_2;

uint8_t EEMEM EEPROM_BALLS_PER_SECOND_3;
uint8_t EEMEM EEPROM_FIRING_MODE_3;
uint8_t EEMEM EEPROM_BURST_SIZE_3;
uint8_t EEMEM EEPROM_AMMO_LIMIT_3;
uint8_t EEMEM EEPROM_SAFETY_SHOT_3;

uint8_t EEPROM_BALLS_PER_SECOND[MAX_PRESETS];
uint8_t EEPROM_FIRING_MODE[MAX_PRESETS];
uint8_t EEPROM_BURST_SIZE[MAX_PRESETS];
uint8_t EEPROM_AMMO_LIMIT[MAX_PRESETS];
uint8_t EEPROM_SAFETY_SHOT[MAX_PRESETS];

uint8_t BALLS_PER_SECOND;
uint8_t FIRING_MODE;
uint8_t BURST_SIZE;
uint8_t AMMO_LIMIT;
uint8_t SAFETY_SHOT;

uint8_t ROUND_DELAY; // delay between shots in ms
uint8_t RELEASE_DEBOUNCE;  // Debounce in ms
uint8_t DWELL;

uint8_t shotsFired;
