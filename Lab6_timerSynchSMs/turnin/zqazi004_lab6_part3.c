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

unsigned char count = 0;

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

enum States {Start, Wait, Add_Press, Sub_Press, Reset} state;

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
    
    unsigned char tempA = ~PINA;
    unsigned char tempB = PORTB;

    switch(state) {
        case Start:
            state = Wait;
            break;

        case Wait:
            if (tempA == 1) 
                state = Add_Press;
            else if (tempA == 2)
                state = Sub_Press;
            else if (tempA == 3)
                state = Reset;
            else if (tempA == 0)
                state = Wait;
            break;

        case Add_Press:
            if (tempA == 1)
               state = Add_Press;
            else if (tempA == 3)
                state = Reset;
            else 
                state = Wait;
            break;

        case Sub_Press:
            if (tempA == 2) {
                state = Sub_Press;
            }
            else if (tempA == 3)
                state = Reset;
            else
                state = Wait;
            break;

        case Reset:
            state = Wait;
            break;

    }

    switch(state) {
        case Start:
            tempB = 0x07;
            break;

        case Add_Press:
            if (tempB < 9 && count == 10) {
                tempB = tempB + 1;
                count = 0;
            }
            count += 1;
            break;

        case Sub_Press:
            if (tempB > 0 && count == 10) {
                tempB = tempB - 1;
                count = 0;
            }
            count++;
            break;

        case Reset:
            count = 0;
            tempB = 0x00;
            break;

        case Wait:
            count = 10;
            break;

        default:
            break;
    }

    PORTB = tempB;

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x07;
    TimerSet(100);
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
