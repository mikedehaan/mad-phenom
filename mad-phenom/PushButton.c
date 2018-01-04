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
#include "PushButton.h"
#include "Common.h"
#include "Globals.h"

bool pushbutton_down = false;
uint32_t pushbutton_activeTime = 0;
uint32_t pushbutton_indicatorTime = 0;
uint8_t pushbutton_currentBlink = 0;
bool pushbutton_indicatorOn = false;

void pushbutton_run(volatile uint32_t *millis) {
	
	bool pastDebounce = ((*millis) - pushbutton_activeTime) > PULL_DEBOUNCE;

	// Check if the push button was pushed
	if (!pushbutton_down && pushButtonHasInput() && pastDebounce) {

		pushbutton_down = true;
		redOn();
		pushbutton_activeTime = (*millis);
		greenOff();
	}
	
#ifdef X7CLASSIC
	if (pushbutton_down
		&& pushButtonHasInput() // Button Held
		&& pastDebounce) {

		// This is used to power down the X7 classic
		if (((*millis) - pushbutton_activeTime) > 5000) {
			// Power down
			//PORTA &= ~(1 << PINA0); // 13 - LOW
			PORTA &= ~(1 << PINA3); // 10 - LOW
			
			// The board should be off by now. Exit routine.
			return;
		}
	}
#endif

	// Has the pushbutton been released? ()
	if (pushbutton_down && !pushButtonHasInput() && pastDebounce) {
		if (((*millis) - pushbutton_activeTime) > 100) {
			togglePreset();
			pushbutton_currentBlink = 0;
			pushbutton_indicatorOn   = false;
			pushbutton_indicatorTime = (*millis);
		}

		pushbutton_down       = false;
		pushbutton_activeTime = (*millis);
		redOff();
	}
	
	// This code will turn the green LED on and off to signify which preset is active
	if (!pushbutton_indicatorOn && ((*millis) - pushbutton_indicatorTime) > 200 && pushbutton_currentBlink < (CURRENT_PRESET + 1)) {
		if (AMMO_LIMIT > 0 && shotsFired >= AMMO_LIMIT) {
			redOn();
		} else {
			greenOn();
		}
		pushbutton_indicatorOn = true;
		pushbutton_indicatorTime = (*millis);
		pushbutton_currentBlink++;
	}
	
	if (pushbutton_indicatorOn && ((*millis) - pushbutton_indicatorTime) > 200) {
		greenOff();
		redOff();
		pushbutton_indicatorOn = false;
		pushbutton_indicatorTime = (*millis);
	}

	if (!pushbutton_indicatorOn && ((*millis) - pushbutton_indicatorTime) > 1000 && pushbutton_currentBlink >= (CURRENT_PRESET + 1)) {
		pushbutton_currentBlink = 0;
	}

#ifdef X7CLASSIC
	// Check the selector switch
	if ((PINB & (1 << PINB0)) <= 0) {
		if (currentSelector != 1) { // Mode was "F", switching to "FA"
			currentSelector = 1; // Mode (FA)
			loadPreset();
		}
	} else {
		if (currentSelector != 0) { // Mode was "FA", switching to "F"
			currentSelector = 0; // Mode (F)
			loadPreset();
		}
	}
#endif

}