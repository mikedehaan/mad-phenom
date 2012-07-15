/*
 * IdleState.cpp
 *
 * Created: 7/12/2012 11:03:58 PM
 *  Author: mike
 */ 
#include "TriggerState.h"
#include "Globals.h"
#include "Common.h"
#include "SolenoidState.h"

TriggerState::TriggerState() {
	triggerPulled = false;
	activeTime = 0;
	lastUpdateTime = 0;
	done = false;
	SolenoidState newSolenoid;
	solenoid = &newSolenoid;
}

void TriggerState::run(volatile uint32_t millisecond) {
	
	// Trigger Pulled
	if (triggerPulled == false && (pinHasInput(TRIGGER_PIN_1) || pinHasInput(TRIGGER_PIN_2))) {
		triggerPulled = true;
		activeTime = millisecond;
		solenoid->reset();
		solenoid->run(millisecond);
	}
	
	// Trigger Held
	if (triggerPulled == true && (pinHasInput(TRIGGER_PIN_1) || pinHasInput(TRIGGER_PIN_2)) && ((millisecond - activeTime) >= PULL_DEBOUNCE)) {
		
	}	
	
	// Trigger Release
	if (triggerPulled == true && (!pinHasInput(TRIGGER_PIN_1) && !pinHasInput(TRIGGER_PIN_2)) && ((millisecond - activeTime) >= PULL_DEBOUNCE)) {
		triggerPulled = false;
		
	}
	

	solenoid->run(millisecond);
};