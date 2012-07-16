/*
 * Trigger.c
 *
 * Created: 7/15/2012 4:07:00 PM
 *  Author: mike
 */
#include <stdbool.h>
#include "Globals.h"
#include "Common.h"
#include "Solenoid.h"

uint32_t trigger_activeTime = 0;
char trigger_roundsFired = 0;
bool trigger_pulled = false;
bool trigger_burstComplete = true;

void trigger_fullAuto(uint32_t *millis);
void trigger_autoResponse(uint32_t *millis);
void trigger_burst(uint32_t *millis);
void trigger_roundComplete();

void (*fireMethod)(uint32_t *millis) = &trigger_fullAuto;

void trigger_run(uint32_t *millis) {
	fireMethod(millis);
}

void trigger_changeMode() {
	// Stop any active firing
	trigger_burstComplete = true;
	trigger_pulled = false;
	
	// 0 - Full Auto
	// 1 - Burst
	// 2 - Auto Response
	if (FIRING_MODE == 0) {
		fireMethod = &trigger_fullAuto;
	} else if (FIRING_MODE == 1) {
		fireMethod = &trigger_burst;
	} else {
		fireMethod = &trigger_autoResponse;
	}
}

void trigger_fullAuto(uint32_t *millis) {

	// Trigger Pulled
	if (!trigger_pulled && (pinHasInput(TRIGGER_PIN_1) || pinHasInput(TRIGGER_PIN_2))) {
		
		trigger_pulled = true;
		trigger_activeTime = (*millis);
		trigger_burstComplete = false;
		solenoid_reset();
	}

	// Trigger Held
	if (trigger_pulled == true &&
		(pinHasInput(TRIGGER_PIN_1) || pinHasInput(TRIGGER_PIN_2)) &&
		(((*millis) - trigger_activeTime) >= PULL_DEBOUNCE) &&
		(((*millis) - trigger_activeTime) >= ROUND_DELAY)) {
	
		trigger_activeTime = (*millis);	
		solenoid_reset();
	}

	// Trigger Release
	if (trigger_pulled == true && (!pinHasInput(TRIGGER_PIN_1) && !pinHasInput(TRIGGER_PIN_2)) && (((*millis) - trigger_activeTime) >= PULL_DEBOUNCE)) {

		trigger_pulled = false;
	}

	solenoid_run(millis);
}

void trigger_autoResponse(uint32_t *millis) {

	// Trigger Pulled
	if (!trigger_pulled && 
		(pinHasInput(TRIGGER_PIN_1) || pinHasInput(TRIGGER_PIN_2)) && 
		(((*millis) - trigger_activeTime) >= RELEASE_DEBOUNCE)) {
			
		trigger_pulled = true;
		trigger_activeTime = (*millis);
		solenoid_reset();
		solenoid_run(millis);
	}

	// Trigger Release
	if (trigger_pulled == true && 
		(!pinHasInput(TRIGGER_PIN_1) && !pinHasInput(TRIGGER_PIN_2)) && 
		(((*millis) - trigger_activeTime) >= PULL_DEBOUNCE)) {
			
		trigger_pulled = false;
		trigger_activeTime = (*millis);
		
		solenoid_reset();
	}

	solenoid_run(millis);
}

void trigger_burst(uint32_t *millis) {

	// Trigger Pulled
	if (!trigger_pulled &&
		(pinHasInput(TRIGGER_PIN_1) || pinHasInput(TRIGGER_PIN_2)) &&
		(((*millis) - trigger_activeTime) >= RELEASE_DEBOUNCE)) {
		
		trigger_pulled = true;
		trigger_activeTime = (*millis);
		trigger_burstComplete = false;
		trigger_roundsFired = 0;
		solenoid_reset();
	}

	// Trigger Release
	if (trigger_pulled == true &&
		(!pinHasInput(TRIGGER_PIN_1) && !pinHasInput(TRIGGER_PIN_2)) &&
		(((*millis) - trigger_activeTime) >= PULL_DEBOUNCE)) {
		
		trigger_pulled = false;
		trigger_activeTime = (*millis);
	}

	if (!trigger_burstComplete && 
		(((*millis) - trigger_activeTime) >= ROUND_DELAY)) {

		trigger_activeTime = (*millis);
		solenoid_reset();
	}

	solenoid_run_callback(millis, &trigger_roundComplete);
}

void trigger_roundComplete() {
	trigger_roundsFired++;
	if (trigger_roundsFired == BURST_SIZE) {
		trigger_burstComplete = true;
	}
}