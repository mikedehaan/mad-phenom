/*
 * Solenoid.h
 *
 * Created: 7/15/2012 4:13:05 PM
 *  Author: mike
 */ 


#ifndef SOLENOID_H_
#define SOLENOID_H_

#include <avr/io.h>

void solenoid_run(volatile uint32_t millis);
void solenoid_reset();

#endif /* SOLENOID_H_ */