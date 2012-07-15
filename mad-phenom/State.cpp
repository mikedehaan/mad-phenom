/*
 * State.cpp
 *
 * Created: 7/12/2012 9:21:31 PM
 *  Author: mike
 */

#include <avr/io.h>
#include <stdbool.h>
#include "State.h"
#include "Common.h"
#include "Globals.h"

State::State() {
	lastUpdateTime = 0;	
}

void State::run(volatile uint32_t &millisecond) {
	static uint8_t pinState = 0;
	
	uint32_t currentTime = millisecond;
	
	if ((currentTime - lastUpdateTime) >= 520) {
		//pinOutput(SOLENOID_PIN, HIGH);
	}		
	
	if ((currentTime - lastUpdateTime) >= 500) {
		if (pinState == 0) {
			pinOutput(11, HIGH);
			pinState = 1;
		} else {
			pinOutput(11, LOW);
			pinState = 0;
		}
		lastUpdateTime = currentTime;
	}
};
