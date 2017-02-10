/*
This file is part of mad-phenom.

mad-phenom is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

mad-phenom is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with mad-phenom.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
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
	
	DDRA |= (1 << PINA1); // Pin 12 - Red LED
	DDRA |= (1 << PINA2); // Pin 11 - Green LED
	DDRA |= (1 << PINA7); // Pin 6  - Solenoid
	
	DDRB &= ~(1 << PINB1); // Pin 3 - Push button
	DDRB &= ~(1 << PINB2); // Pin 5 - Trigger Pin 1
	DDRA &= ~(1 << PINA6); // Pin 7 - Trigger Pin 2

	// Other unknown pins
	//DDRA |= (1 << PINA0); // Pin 13
	//PORTA |= (1 << PINA0); // 13 - HIGH
#ifdef X7CLASSIC
	DDRA |= (1 << PINA3); // Pin 10 - Power pin
	PORTA |= (1 << PINA3); // 10 - HIGH
#endif
	//PORTA &= ~(1 << PINA4);	// 9 - LOW
	//PORTA &= ~(1 << PINA5);	// 8 - LOW
	
	// Set Triggers HIGH
	PORTB |= (1 << PINB2);
	PORTA |= (1 << PINA6);
	
	// Set Pushbutton HIGH
	PORTB |= (1 << PINB1);

#ifdef X7CLASSIC
	// Selector switch on pin 2
	DDRB &= ~(1 << PINB0); // Pin 2
	PORTB |= (1 << PINB0); // Pin 2 set HIGH
#endif

	while (true) {
		// Check the selector switch
		if ((PINB & (1 << PINB0)) <= 0) {
			if (currentSelector != 1) { // Mode was "F", switching to "FA"
				currentSelector = 1; // Mode (FA - Red)
				greenOff();
				redOn();
			}
		} else {
			if (currentSelector != 0) { // Mode was "FA", switching to "F"
				currentSelector = 0; // Mode (F - Green)
				redOff();
				greenOn();
			}
		}
	}
}