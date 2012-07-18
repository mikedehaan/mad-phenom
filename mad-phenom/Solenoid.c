/*
 * Solenoid.c
 *
 * Created: 7/15/2012 4:12:55 PM
 *  Author: mike
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
		shotsFired++;
		pinOutput(PIN_SOLENOID, HIGH);
		activeTime = (*millis);
		solenoidActive = true;
	}
	
	if (solenoidActive && (((*millis) - activeTime) >= DWELL)) {
		pinOutput(PIN_SOLENOID, LOW);
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