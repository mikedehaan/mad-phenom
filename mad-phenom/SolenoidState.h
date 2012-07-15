/*
 * SolenoidState.h
 *
 * Created: 7/14/2012 5:55:01 PM
 *  Author: mike
 */ 
#ifndef SOLENOIDSTATE_H_
#define SOLENOIDSTATE_H_

#include <avr/io.h>
#include <stdbool.h>
#include "State.h"
#include "Globals.h"

class SolenoidState {
	bool isActive;
	uint32_t activeTime;
	bool done;	
public:
	SolenoidState();
	void run(volatile uint32_t millisecond);
	bool isDone();
	void reset();	
};

#endif /* SOLENOIDSTATE_H_ */