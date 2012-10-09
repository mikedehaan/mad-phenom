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
uint32_t queue_activeTime = 0;
uint32_t lastTriggerPullTime = 0;
uint8_t safetyShotsFired = 0;
bool trigger_pulled = false;
uint8_t firing_queue = 0;

void trigger_singleShot(uint32_t *millis);
void trigger_fullAuto(uint32_t *millis);
void trigger_autoResponse(uint32_t *millis);
void trigger_burst(uint32_t *millis);
void fireFromQueue(uint32_t *millis);

void (*fireMethod)(uint32_t *millis) = &trigger_fullAuto;

bool triggerHeld() {
	return ((PINB & (1 << PINB2)) <= 0) || ((PINA & (1 << PINA6)) <= 0);
}

bool triggerReleased() {
	return ((PINB & (1 << PINB2)) > 0) && ((PINA & (1 << PINA6)) > 0);
}

void trigger_run(uint32_t *millis) {
	fireMethod(millis);
}

bool checkPullDebounce(uint32_t *millis) {
	return (((*millis) - trigger_activeTime) >= PULL_DEBOUNCE);
}

bool checkReleaseDebounce(uint32_t *millis) {
	return (((*millis) - trigger_activeTime) >= RELEASE_DEBOUNCE);
}

void trigger_changeMode() {
	// Stop any active firing
	trigger_pulled = false;
	
	// 0 - Full Auto
	// 1 - Burst
	// 2 - Auto Response
	if (FIRING_MODE == 0) {
		fireMethod = &trigger_fullAuto;
	} else if (FIRING_MODE == 1) {
		fireMethod = &trigger_burst;
	} else if (FIRING_MODE == 2) {
		fireMethod = &trigger_autoResponse;
	} else {
		fireMethod = &trigger_singleShot;
	}
}

void trigger_singleShot(uint32_t *millis) {

	// Trigger Pulled
	if (!trigger_pulled &&
		triggerHeld() &&
		checkReleaseDebounce(millis)) {
		
		trigger_pulled = true;
		trigger_activeTime = (*millis);
		firing_queue = 1;
	}

	// Trigger Release
	if (trigger_pulled == true && triggerReleased() && checkPullDebounce(millis)) {

		trigger_pulled = false;
		trigger_activeTime = (*millis);
	}


	fireFromQueue(millis);
}

void trigger_fullAuto(uint32_t *millis) {

	// Trigger Pulled
	if (!trigger_pulled && 
		triggerHeld() &&
		checkReleaseDebounce(millis)) {

		trigger_pulled = true;
		trigger_activeTime = (*millis);
		firing_queue = 1;
	}

	// Trigger Held
	if (trigger_pulled == true &&
		triggerHeld() &&
		checkPullDebounce(millis) &&
		(((*millis) - trigger_activeTime) >= ROUND_DELAY)) {
	
		trigger_activeTime = (*millis);
		
		// Don't allow FA if safety shots have not been reached
		if (safetyShotsFired >= SAFETY_SHOT || SAFETY_SHOT == 0) {
			firing_queue = 1;
		}			
	}

	// Trigger Release
	if (trigger_pulled && triggerReleased() && checkPullDebounce(millis)) {
		trigger_pulled = false;
	}

	fireFromQueue(millis);
}

void trigger_autoResponse(uint32_t *millis) {

	// Trigger Pulled
	if (!trigger_pulled && 
		triggerHeld() && 
		checkReleaseDebounce(millis)) {
			
		trigger_pulled = true;
		trigger_activeTime = (*millis);
		firing_queue++;
	}

	// Trigger Release
	if (trigger_pulled == true && 
		triggerReleased() && 
		checkPullDebounce(millis)) {

		// Don't allow auto response if safety shots have not been reached
		if (safetyShotsFired >= SAFETY_SHOT || SAFETY_SHOT == 0) {
			firing_queue++;
		}			

		trigger_pulled = false;
		trigger_activeTime = (*millis);
	}

	if (firing_queue > 2) {
		firing_queue = 2;
	}

	fireFromQueue(millis);
}

void trigger_burst(uint32_t *millis) {

	// Trigger Pulled
	if (!trigger_pulled &&
		triggerHeld() &&
		checkPullDebounce(millis)) {
		
		trigger_pulled = true;
		trigger_activeTime = (*millis);
		
		// Don't allow burst if safety shots have not been reached
		if (safetyShotsFired >= SAFETY_SHOT || SAFETY_SHOT == 0) {
			firing_queue = BURST_SIZE;
		} else {
			firing_queue = 1;
		}
	}

	// Trigger Release
	if (trigger_pulled == true &&
		triggerReleased() &&
		checkPullDebounce(millis)) {
		
		trigger_pulled = false;
		trigger_activeTime = (*millis);
	}

	fireFromQueue(millis);
}


// Semi Auto, set queue to 1
// Full Auto, while the trigger is pulled, set queue to 1
// round burst, set queue to BURST_SIZE
// Auto-response, set queue to 1 on trigger pull and again on release

void fireFromQueue(uint32_t *millis) {
	if (firing_queue > 0 && ((*millis) - queue_activeTime >= ROUND_DELAY)) {
		// decrement the queue
		firing_queue--;

		// Fire a round
		solenoid_reset();

		// Reset the trigger active time
		queue_activeTime = (*millis);
		
		// If the ball was fired within a second, increment safety shots fired
		if ((*millis) - lastTriggerPullTime <= 1000) {
			safetyShotsFired++;
			
			if (safetyShotsFired >= SAFETY_SHOT) {
				safetyShotsFired = SAFETY_SHOT;
			}
		} else {
			safetyShotsFired = 0;
		}
		
		lastTriggerPullTime = (*millis);	
	}

	solenoid_run(millis);
}
