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
#include "Globals.h"
#include "Common.h"
#include "Solenoid.h"

uint32_t trigger_activeTime = 0;
uint32_t trigger_heldTime = 0;
uint32_t queue_activeTime = 0;
uint32_t lastTriggerPullTime = 0;
uint8_t safetyShotsFired = 0;
bool trigger_pulled = false;
uint8_t firing_queue = 0;

//void trigger_singleShot(uint32_t *millis);
//void trigger_fullAuto(uint32_t *millis);
//void trigger_autoResponse(uint32_t *millis);
//void trigger_burst(uint32_t *millis);
void fireFromQueue(uint32_t *millis);
void trigger_action(uint32_t *millis);

void (*fireMethod)(uint32_t *millis) = &trigger_action;

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
	
	/*
	// 0 - Full Auto
	// 1 - Burst
	// 2 - Auto Response
	if (FIRING_MODE == 0) {        // FULL AUTO
		fireMethod = &trigger_fullAuto;
	} else if (FIRING_MODE == 1) { // BURST
		fireMethod = &trigger_burst;
	} else if (FIRING_MODE == 2) { // AUTO RESPONSE
		fireMethod = &trigger_autoResponse;
	} else {                       // SINGLE SHOT
		fireMethod = &trigger_singleShot;
	}
	*/
	
	fireMethod = &trigger_action;
}
/*
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
		// FA needs to be greater than the safety shot
		// since holding the trigger would auto-qualify the last safety shot.
		if (safetyShotsFired > SAFETY_SHOT || SAFETY_SHOT == 0) {
			firing_queue = 1;
		}			
	}

	// Trigger Release
	if (trigger_pulled && triggerReleased() && checkPullDebounce(millis)) {
		trigger_pulled = false;
		trigger_activeTime = (*millis);
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
*/
void trigger_action(uint32_t *millis) {
	
	//////// TRIGGER PULLED
	
	// NOTE: Burst originally used checkPullDebounce()
	if (!trigger_pulled && triggerHeld() &&	checkReleaseDebounce(millis)) {

		trigger_pulled = true;
		trigger_activeTime = (*millis);
		trigger_heldTime = (*millis);  // How long the trigger was held for
		
		switch (FIRING_MODE) {
			case 0: // Full Auto
				firing_queue = 1;
				break;
			case 1: // Burst
				// Don't allow burst if safety shots have not been reached
				if (safetyShotsFired >= SAFETY_SHOT || SAFETY_SHOT == 0) {
					firing_queue = BURST_SIZE;
				} else {
					firing_queue = 1;
				}			
				break;
			case 2: // Auto Response
				firing_queue++;			
				break;
			default:  // Single Shot
				firing_queue = 1;			
				break;
		}
	}
		
	//////// TRIGGER HELD
	// Trigger Held
	if (trigger_pulled && triggerHeld() && checkPullDebounce(millis) && (((*millis) - trigger_activeTime) >= ROUND_DELAY)) {
		
		switch (FIRING_MODE) {
			case 0: // Full Auto
				trigger_activeTime = (*millis);
		
				// Don't allow FA if safety shots have not been reached
				// FA needs to be greater than the safety shot
				// since holding the trigger would auto-qualify the last safety shot.
				if (safetyShotsFired > SAFETY_SHOT || SAFETY_SHOT == 0) {
					firing_queue = 1;
				}
				break;
			case 1:
				// Nothing while trigger is held
				break;
			case 2:
				// Nothing while trigger is held
				break;
			default:
				// Nothing while trigger is held
				break;
		}
		
		// If AMMO LIMIT is enabled and the trigger is held down for more than 2 seconds, reset the ammo limit
		// For now, I'm leaving this enabled for full-auto as well (we'll see how the user feedback goes).
		if (AMMO_LIMIT > 0 && shotsFired >= AMMO_LIMIT && ((*millis) - trigger_heldTime) >= 2000) {
			// Reset the ammo limit
			shotsFired = 0;
		}
	}
	
	//////// TRIGGER RELEASED
	// Trigger Release
	if (trigger_pulled && triggerReleased() && checkPullDebounce(millis)) {
		trigger_pulled = false;
		trigger_activeTime = (*millis);

		// Fire a round if Auto response
		if (FIRING_MODE == 2) {

			// If the trigger was held for 2 seconds or more, don't fire a round
			if (((*millis) - trigger_heldTime) < 2000) {

				// Don't allow auto response if safety shots have not been reached
				if (safetyShotsFired >= SAFETY_SHOT || SAFETY_SHOT == 0) {
					firing_queue++;
				}
			}
		}
	}

	// FIRE!!!
	fireFromQueue(millis);	
}

// Semi Auto, set queue to 1
// Full Auto, while the trigger is pulled, set queue to 1
// round burst, set queue to BURST_SIZE
// Auto-response, set queue to 1 on trigger pull and again on release

void fireFromQueue(uint32_t *millis) {
	if (firing_queue > 0 && ((*millis) - queue_activeTime >= ROUND_DELAY)) {
		
		lastTriggerPullTime = (*millis);

		safetyShotsFired++;

		uint8_t safetyMax = SAFETY_SHOT + 1;

		// This is a safety to prevent integer overflow (adding 1 for FA)
		if (safetyShotsFired > (safetyMax)) {
			safetyShotsFired = safetyMax;
		}

		// decrement the queue
		firing_queue--;

		// Fire a round
		solenoid_reset();

		// Reset the trigger active time
		queue_activeTime = (*millis);
		
	}

	// If the ball was fired within a second, increment safety shots fired
	if ((*millis) - lastTriggerPullTime > 1000) {
		safetyShotsFired = 0;
	}

	solenoid_run(millis);
}
