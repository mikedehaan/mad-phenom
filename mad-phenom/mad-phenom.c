/*
 * mad_phenom.c
 *
 * Created: 7/15/2012 3:52:15 PM
 *  Author: mike
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "Globals.h"
#include "Common.h"
#include "Menu.h"
#include "Trigger.h"

volatile uint32_t millis = 0;
uint8_t counter = 0;

// This interrupt should occur approx. 3906 times per second
// divide by 4 to get an approx millisecond
ISR(TIM0_COMPA_vect) {
	TCNT0 = 0;
	counter++;
	if (counter == 4) {
		millis++;
		counter = 0;
	}
}

int main(void) {
	
	TCCR0B |= (1 << CS01);  // Enable timer with 1/8th prescale
	TIMSK0 |= 1 << OCIE0A; // Configure Timer0 for Compare Match
	OCR0A = 255; // Match at 200
	//DDRA |= 1 << 2; // Set pin 11 as output
	//DDRA |= 1 << 7; // Set pin 6 as output
	sei();  // Enable global interrupts	
	
	initialize();
	
	setOutputPin(PIN_LED_RED);
	setOutputPin(PIN_LED_GREEN);
	setInputPin(TRIGGER_PIN_1);
	setInputPin(TRIGGER_PIN_2);
	setOutputPin(PIN_SOLENOID);
	setInputPin(PIN_PUSHBUTTON); // Push button	
	
	pinOutput(TRIGGER_PIN_1, HIGH);
	pinOutput(TRIGGER_PIN_2, HIGH);
	
    while(1) {
        trigger_run(millis);
    }
}