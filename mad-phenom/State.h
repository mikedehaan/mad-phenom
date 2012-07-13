/*
 * State.h
 *
 * Created: 7/12/2012 9:31:11 PM
 *  Author: mike
 */ 


#ifndef STATE_H_
#define STATE_H_

#include <avr/io.h>
#include <stdbool.h>

class State {
protected:
	uint32_t lastUpdateTime;
	bool done;
public:
	State();
	void run(uint32_t currentTime);
	bool isDone();
};

#endif /* STATE_H_ */