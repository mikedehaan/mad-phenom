/*
 * mad_phenom.cpp
 *
 * Created: 7/12/2012 11:26:12 PM
 *  Author: mike
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "IdleState.h"
#include "Common.h"
#include "Globals.h"

#define F_CPU 8000000UL

volatile uint32_t currentTimeInMillis = 0;

ISR(TIM0_COMPA_vect) {
	OCR0A += 200;
	currentTimeInMillis++;
}

int main(void) {
	
	// Prescale the time to 1/8th of the system clock
	//TCCR0B |= (1 << CS01);
	
	// Enable timer with no prescale
	//TCCR0B |= (1 << CS00);

	OCR0A = 200;

	// Enable timer with 1/64th
	TCCR0B |= (1 << CS00) | (1 << CS01);
	
	// enable timer overflow interrupt
	//TIMSK0 |= 1 << TOIE0;
	TIMSK0 |= 1 << OCIE0A;
	
	
	// Set pin 6 for output
	setOutputPin(11);
	pinOutput(11, LOW);
	
	sei();
	
	IdleState state;

	while(1) {
		state.run(currentTimeInMillis);
	}
}