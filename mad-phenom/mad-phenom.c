/*
 * mad_phenom.c
 *
 * Created: 7/15/2012 3:52:15 PM
 *  Author: mike
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdbool.h>

#include "Globals.h"
#include "Common.h"
#include "Menu.h"
#include "Trigger.h"
#include "PushButton.h"

volatile uint32_t millis = 0;
uint8_t counter = 0;
bool triggerPulled = false;

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
	
	sei();  // Enable global interrupts
	
	initialize();
	
	setOutputPin(PIN_LED_RED);
	setOutputPin(PIN_LED_GREEN);
	setOutputPin(PIN_SOLENOID);
	//DDRA |= (1 << PINA0); // DEBUG PIN
	//DDRA |= (1 << PINA3); // DEBUG PIN

	setInputPin(TRIGGER_PIN_1);
	setInputPin(TRIGGER_PIN_2);
	setInputPin(PIN_PUSHBUTTON); // Push button	
	
	// Other unknown pins
	PORTA |= (1 << PINA0); // 13 - HIGH
	PORTA |= (1 << PINA3); // 10 - HIGH
	PORTA &= ~(1 << PINA4);	// 9 - LOW
	PORTA &= ~(1 << PINA5);	// 8 - LOW
	
	// Set Triggers HIGH
	//pinOutput(TRIGGER_PIN_1, HIGH);
	//pinOutput(TRIGGER_PIN_2, HIGH);
	PORTB |= (1 << PINB2);
	PORTA |= (1 << PINA6);
	
	// Set Pushbutton HIGH
	//pinOutput(PIN_PUSHBUTTON, HIGH);
	PORTB |= (1 << PINB1);
	
	// If the button is held during startup, enter config mode.
	uint16_t buttonHeldTime = 0;
	bool configMode = false;
	while (pinHasInput(3)) {
		delay_ms(1);
		
		buttonHeldTime++;
		if (buttonHeldTime > 3000) {
			buttonHeldTime = 3000;
		}
	}
	
	if (buttonHeldTime >= 1000) {
		configMode = true;
		
		// Some firing modes have 0 PULL_DEBOUNCE
		// Reset so the menus work.
		PULL_DEBOUNCE = RELEASE_DEBOUNCE;
	}	
	
	/*
	// If the button is held during startup, enter config mode.
	uint32_t buttonHeldTime = millis;
	while (pinHasInput(3)) {
		// don't do anything
	}
	
	bool configMode = false;
	if ((millis - buttonHeldTime) >= 1000) {
		configMode = true;
			
		// Some firing modes have 0 PULL_DEBOUNCE
		// Reset so the menus work.
		PULL_DEBOUNCE = RELEASE_DEBOUNCE;
	}
	*/
	
	if (configMode) {
		// Initialize interrupts for the menu system
		PCMSK1 |= (1 << PCINT10);  //Enable interrupts on PCINT10 (trigger)
		PCMSK1 |= (1 << PCINT9);  // Enable interrupts for the push button
		GIMSK = (1 << PCIE1);    //Enable interrupts period for PCI0 (PCINT11:8
		
		handleConfig();	
	} else { // Normal run mode
		for (;;) {
			// This prevents time from changing within an iteration
			//uint32_t millisCopy = millis;
			trigger_run(&millis);
			pushbutton_run(&millis);
		}
	}		
}

ISR(PCINT1_vect) {
	if (!triggerPulled && triggerHeld()) {
		triggerPulled = true;

		uint16_t buttonHeldTime = 0;
		delay_ms(PULL_DEBOUNCE);
		while (triggerHeld()) {
			delay_ms(1);
			buttonHeldTime += 1;
			
			if (buttonHeldTime > 3000) {
				buttonHeldTime = 3000;
			}
		}
		configTriggerPulled(buttonHeldTime);
	}

	if (triggerPulled && triggerReleased()) {		
		delay_ms(RELEASE_DEBOUNCE);
		triggerPulled = false;
	}
}