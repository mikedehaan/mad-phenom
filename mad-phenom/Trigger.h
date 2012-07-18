/*
 * Trigger.h
 *
 * Created: 7/15/2012 4:07:27 PM
 *  Author: mike
 */ 


#ifndef TRIGGER_H_
#define TRIGGER_H_

#include <avr/io.h>
#include <stdbool.h>

void trigger_run(uint32_t *millisecond);
void trigger_changeMode();
bool triggerHeld();
bool triggerReleased();

#endif /* TRIGGER_H_ */