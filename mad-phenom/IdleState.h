/*
 * IdleState.h
 *
 * Created: 7/12/2012 11:04:21 PM
 *  Author: mike
 */ 


#ifndef IDLESTATE_H_
#define IDLESTATE_H_


#include <avr/io.h>
#include <stdbool.h>
#include "State.h"

class IdleState: public State {
public:
	IdleState();
};


#endif /* IDLESTATE_H_ */