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
int getPinMask(uint8_t pinNumber);
void setInputPin(int pinNumber);
void setOutputPin(int pinNumber);
void pinOutput(int pinNumber, int state);
uint8_t pinHasInput(uint8_t pinNumber);
void initialize();
void threeRoundBurst();
void togglePreset();


#endif /* COMMON_H_ */