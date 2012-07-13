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

extern void (*fireMethod)();
extern void (*fireOnTriggerRelease)();

void loadPreset();
int getPinMask(int pinNumber);
void setInputPin(int pinNumber);
void setOutputPin(int pinNumber);
void pinOutput(int pinNumber, int state);
void pinToggle(int pinNumber);
int pinHasInput(int pinNumber);
void initialize();
void threeRoundBurst();
void fullAuto();
void singleShot();


#endif /* COMMON_H_ */