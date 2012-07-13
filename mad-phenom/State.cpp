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

State::State() {
	done = false;
	lastUpdateTime = 0;	
}

void State::run(uint32_t currentTime) {
	static uint8_t pinState = 0;
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

bool State::isDone() {
	return done;
}
