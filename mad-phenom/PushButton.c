/*
 * PushButton.c
 *
 * Created: 7/15/2012 11:08:00 PM
 *  Author: mike
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
	
	// Check if the push button was pushed
	if (!pushbutton_down && pushButtonHasInput() && ((*millis) - pushbutton_activeTime) > PULL_DEBOUNCE) {
		pushbutton_down = true;
		redOn();
		pushbutton_activeTime = (*millis);
		
		togglePreset();
		
		pushbutton_currentBlink = 0;
		pushbutton_indicatorOn = false;
		pushbutton_indicatorTime = (*millis);
		greenOff();
	}
	
	// Has the pushbutton been released?
	if (pushbutton_down && !pushButtonHasInput() && ((*millis) - pushbutton_activeTime) > PULL_DEBOUNCE) {
		pushbutton_down = false;
		pushbutton_activeTime = (*millis);
		redOff();
	}
	
	// This code will turn the green LED on and off to signify which preset is active
	if (!pushbutton_indicatorOn && ((*millis) - pushbutton_indicatorTime) > 200 && pushbutton_currentBlink != (CURRENT_PRESET + 1)) {
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
	
	if (!pushbutton_indicatorOn && ((*millis) - pushbutton_indicatorTime) > 1000 && pushbutton_currentBlink == (CURRENT_PRESET + 1)) {
		pushbutton_currentBlink = 0;
	}
}