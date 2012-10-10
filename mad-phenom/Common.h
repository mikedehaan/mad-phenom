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
#ifndef COMMON_H_
#define COMMON_H_

#include <avr/eeprom.h>
#include <stdbool.h>

#define HIGH 1
#define LOW 0

void loadPreset();
void delay_ms(uint16_t ms);
void initialize();
void togglePreset();

void redOff();
void greenOff();
void redOn();
void greenOn();
void redSet(bool state);
void greenSet(bool state);
void solenoidOn();
void solenoidOff();
bool pushButtonHasInput();

#endif /* COMMON_H_ */