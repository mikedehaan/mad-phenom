/*
 * IdleState.h
 *
 * Created: 7/12/2012 11:04:21 PM
 *  Author: mike
 */ 


#ifndef TRIGGERSTATE_H_
#define TRIGGERSTATE_H_


#include <avr/io.h>
#include <stdbool.h>
#include "State.h"
#include "Globals.h"
#include "SolenoidState.h"

class TriggerState {
	uint32_t lastUpdateTime;
	bool done;
	bool triggerPulled;
	uint32_t activeTime;
	SolenoidState *solenoid;
public:
	TriggerState();
	void run(volatile uint32_t millisecond);
};


#endif /* TRIGGERSTATE_H_ */