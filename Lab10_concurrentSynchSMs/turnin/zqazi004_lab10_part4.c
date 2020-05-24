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
unsigned char speaker = 0;

unsigned long TS_storedTime = 5;

enum TL_States {TL_Start, TL_LED0, TL_LED1, TL_LED2} TL_State;

enum BL_States {BL_Start, BL_1, BL_0} BL_State;

enum TS_States {TS_Start, TS_Wait, TS_1, TS_0} TS_State;

enum CF_States {CF_Start, CF_Wait, CF_Add, CF_Sub} CF_State;

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

void Tick_ToggleSpeaker() {

    unsigned char A2 = ~PINA & 0x04;

    switch (TS_State) {
        case TS_Start:
            TS_State = TS_Wait;
            break;
        
        case TS_Wait:
            TS_State = A2 ? TS_1 : TS_Wait;
            break;

        case TS_1:
            TS_State = A2 ? TS_0 : TS_Wait;
            break;

        case TS_0:
            TS_State = A2 ? TS_1 : TS_Wait;
            break;
    }

    switch (TS_State) {
        case TS_1:
            speaker = 0x10;
            break;

        default:
            speaker = 0;
            break;
    }

}

void Tick_ChangeFrequency() {

    unsigned char A0 = ~PINA & 0x01;
    unsigned char A1 = ~PINA & 0x02;

    switch (CF_State) {
        case CF_Start:
            CF_State = CF_Wait;
            break;

        case CF_Wait:
            if (A0)
                CF_State = CF_Add;
            else if (A1)
                CF_State = CF_Sub;
            else
                CF_State = CF_Wait;
            break;

        default:
            CF_State = CF_Wait;
            break;
    }

    switch (CF_State) {
        case CF_Add:
            TS_storedTime++;
            break;

        case CF_Sub:
            TS_storedTime--;
            break;

        default:
            break;
    }

}

void Tick_CombineLEDsSM() {

    PORTB =  speaker | threeLEDs | blinkingLED;

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x10;

    unsigned long TL_elapsedTime = 300;
    unsigned long BL_elapsedTime = 1000;
    unsigned long CF_elapsedTime = 100;
    unsigned long TS_elapsedTime = 5;

    const unsigned long timerPeriod = 1;

    TimerSet(timerPeriod);
    TimerOn();
    
    TL_State = TL_Start;
    BL_State = BL_Start;
    TS_State = TS_Start;
    CF_State = CF_Start;

    /* Insert your solution below */
    while (1) {
        if (TL_elapsedTime >= 300) {
            Tick_ThreeLEDsSM();
            TL_elapsedTime = 0;
        }
        if (BL_elapsedTime >= 1000) {
            Tick_BlinkingLEDSM();
            BL_elapsedTime = 0;
        }
        if (TS_elapsedTime >= TS_storedTime) {
            Tick_ToggleSpeaker();
            TS_elapsedTime = 0;
        }
        if (CF_elapsedTime >= 100) {
            Tick_ChangeFrequency();
            CF_elapsedTime = 0;
        }
        Tick_CombineLEDsSM();
        while (!TimerFlag);
        TimerFlag = 0;
        TL_elapsedTime += timerPeriod;
        BL_elapsedTime += timerPeriod;
        TS_elapsedTime += timerPeriod;
        CF_elapsedTime += timerPeriod;
    }
    return 1;
}
