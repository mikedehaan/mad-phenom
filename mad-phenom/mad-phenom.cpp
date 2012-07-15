/*
 * mad_phenom.cpp
 *
 * Created: 7/12/2012 11:26:12 PM
 *  Author: mike
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "TriggerState.h"
#include "Common.h"
#include "Globals.h"

#define F_CPU 8000000UL

volatile uint32_t millisecond = 0;
uint8_t counter = 0;

// This interrupt should occur approx. 3906 times per second
// divide by 4 to get an approx millisecond
ISR(TIM0_COMPA_vect) {
	TCNT0 = 0;
	counter++;
	if (counter == 4) {
		millisecond++;
		counter = 0;
	}
}

int main(void) {
	
	TCCR0B |= (1 << CS01);  // Enable timer with 1/8th prescale
	TIMSK0 |= 1 << OCIE0A; // Configure Timer0 for Compare Match
	OCR0A = 255; // Match at 200
	sei();  // Enable global interrupts
	
	// Set pin 6 for output
	setOutputPin(11);
	pinOutput(11, LOW);

	setOutputPin(SOLENOID_PIN);
	pinOutput(SOLENOID_PIN, LOW);
	
	setInputPin(TRIGGER_PIN_1);
	setInputPin(TRIGGER_PIN_2);
	pinOutput(TRIGGER_PIN_1, HIGH);
	pinOutput(TRIGGER_PIN_2, HIGH);
	
	sei();
	
	initialize();
	
	TriggerState state;
	State greenLed;

	while(1) {		
		greenLed.run(millisecond);
		state.run(millisecond);
	}
}