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
#include "Solenoid.h"
#include "Globals.h"
#include "Common.h"

bool solenoidDone = true;
bool solenoidActive = false;
uint32_t activeTime = 0;

void solenoid_run(volatile uint32_t *millis) {
	solenoid_run_callback(millis, 0);
}

void solenoid_run_callback(volatile uint32_t *millis, void (*solenoidCompleteFunction)()) {
	if (solenoidDone) {
		return;
	}
	
	// If the Solenoid is not currently active, and an ammo limit is set, do not fire!
	if (!solenoidActive && AMMO_LIMIT != 0 && shotsFired >= AMMO_LIMIT) {
		solenoidDone = true;
		return;
	}
	
	if (!solenoidActive) {  // Activate the Solenoid
		if (shotsFired < 255) {
			shotsFired++;
		}		

		solenoidOn();
		activeTime = (*millis);
		solenoidActive = true;
	}
	
	if (solenoidActive && (((*millis) - activeTime) >= DWELL)) {
		solenoidOff();
		solenoidDone = true;
		solenoidActive = false;
		if (solenoidCompleteFunction != 0) {
			solenoidCompleteFunction();
		}
	}
}

void solenoid_reset() {
	solenoidDone = false;
}