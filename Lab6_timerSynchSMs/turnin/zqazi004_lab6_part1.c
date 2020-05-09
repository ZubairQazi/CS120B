/*	Author: zubair
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

enum States {Start, One, Two, Three} state;

void TimerOn() {
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;

    _avr_timer_cntcurr = _avr_timer_M;

    SREG |= 0x80;
}

void TimerOff() {
    TCCR1B = 0x00;
}

void TimerISR() {
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
    _avr_timer_cntcurr--;
    if (_avr_timer_cntcurr == 0) {
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M) {
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}

void Tick() {
    switch (state) {
        case Start:
            state = One;
            break;
        
        case One:
            state = Two;
            break;

        case Two:
            state = Three;
            break;

        case Three:
            state = One;
            break;

        default:
            state = Start;
    }

    switch (state) {
        case One:
            PORTB = 0x01;
            break;

        case Two:
            PORTB = 0x02;
            break;

        case Three:
            PORTB = 0x04;
            break;

        default:
            PORTB = 0x01;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(1000);
    TimerOn();
    
    state = Start;
    /* Insert your solution below */
    while (1) {
        // Toggle B0, B1, B2
        Tick();
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
