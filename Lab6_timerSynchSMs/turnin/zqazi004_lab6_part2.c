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

enum States {Start, One, Two, Three, Four, Press, Release} state;

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
    unsigned char A0 = ~PINA & 0x01;
    unsigned char tempB = 0x00;
    switch (state) {
        case Start:
            state = One;
            break;
        
        case One:
            if (A0) {
                tempB = PORTB;
                state = Press;
            }
            else
                state = Two;
            break;

        case Two:
            if (A0){
                tempB = PORTB;
                state = Press;
            }
            else
                state = Three;
            break;

        case Three:
            if (A0){
                tempB = PORTB;
                state = Press;
            }
            else
                state = Four;
            break;

        case Four:
            if (A0){
                tempB = PORTB;
                state = Press;
            }
            else
                state = One;
            break;

        case Press:
            if (A0){
                tempB = PORTB;
                state = Press;
            }
            else if (!A0){
                tempB = PORTB;
                state = Release;
            }
            break;

        case Release:
            if (A0) {
                state = One;
            }
            else if (!A0) {
                tempB = PORTB;
                state = Release;
            }
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

        case Four:
            PORTB = 0x02;
            break;

        case Press:
            PORTB = tempB;
            break;

        case Release:
            PORTB = tempB;
            break;

        default:
            PORTB = 0x00;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(300);
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
