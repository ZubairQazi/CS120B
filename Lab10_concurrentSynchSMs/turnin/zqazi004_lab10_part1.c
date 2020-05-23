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

unsigned char threeLEDs = 0;
unsigned char blinkingLED = 0;

enum TL_States {TL_Start, TL_LED0, TL_LED1, TL_LED2} TL_State;

enum BL_States {BL_Start, BL_1, BL_0} BL_State;

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

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

void Tick_ThreeLEDsSM() {

    switch(TL_State) {
        case TL_Start:
            TL_State = TL_LED0;
            break;

        case TL_LED0:
            TL_State = TL_LED1;
            break;

        case TL_LED1:
            TL_State = TL_LED2;
            break;

        case TL_LED2:
            TL_State = TL_LED0;
            break;
    }

    switch(TL_State) {
        case TL_LED0:
            threeLEDs = 1;
            break;

        case TL_LED1:
            threeLEDs = 2;
            break;

        case TL_LED2:
            threeLEDs = 4;
            break;

        default:
            break;
    }

}

void Tick_BlinkingLEDSM() {

    switch (BL_State) {
        case BL_Start:
            BL_State = BL_1;
            break;

        case BL_1:
            BL_State = BL_0;
            break;

        case BL_0:
            BL_State = BL_1;
            break;
    }

    switch (BL_State) {
        case BL_1:
            blinkingLED = 8;
            break;

        case BL_0:
            blinkingLED = 0;
            break;

        default:
            break;
    }

}

void Tick_CombineLEDsSM() {

    PORTB = threeLEDs | blinkingLED;

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(1000);
    TimerOn();
    
    TL_State = TL_Start;
    BL_State = BL_Start;

    /* Insert your solution below */
    while (1) {
        Tick_ThreeLEDsSM();
        Tick_BlinkingLEDSM();
        Tick_CombineLEDsSM();
        while (!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
