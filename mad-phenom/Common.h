/*
 * IncFile1.h
 *
 * Created: 5/25/2012 11:33:54 PM
 *  Author: mike
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#include <avr/eeprom.h>

#define HIGH 1
#define LOW 0

void loadPreset();
void delay_ms(uint16_t ms );
uint8_t getPinMask(uint8_t pinNumber);
void setInputPin(uint8_t pinNumber);
void setOutputPin(uint8_t pinNumber);
void pinOutput(uint8_t pinNumber, uint8_t state);
uint8_t pinHasInput(uint8_t pinNumber);
void initialize();
void threeRoundBurst();
void togglePreset();


#endif /* COMMON_H_ */