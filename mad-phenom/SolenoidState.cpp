/*
 * CPPFile1.cpp
 *
 * Created: 7/14/2012 5:54:37 PM
 *  Author: mike
 */
#include <stdbool.h>
#include "SolenoidState.h"
#include "Globals.h"
#include "Common.h"

SolenoidState::SolenoidState() {
	isActive = false;
	done = true;
	activeTime = 0;
}

void SolenoidState::run(volatile uint32_t millisecond) {
	
	if (done == true) {
		return;
	}

	if (isActive == false) {
		isActive = true;
		activeTime = millisecond;
		pinOutput(SOLENOID_PIN, HIGH);
	}
	
	if (isActive == true && (millisecond - activeTime) >= DWELL) {
		isActive = false;
		activeTime = 0;
		pinOutput(SOLENOID_PIN, LOW);
		done = true;
	}	
};

bool SolenoidState::isDone() {
	return done;
}

void SolenoidState::reset() {
	done = false;
}