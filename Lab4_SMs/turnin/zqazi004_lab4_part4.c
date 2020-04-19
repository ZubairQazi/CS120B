/*	Author: Zubair
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//#include "RIMS.h"
enum States {Start, Locked, LHash_Press, LHash_Release, Unlocked, UHash_Press, UHash_Release} state;

void Tick() {
    
    unsigned char tempA = PINA;
    unsigned char A0 = PINA & 0x01;
    unsigned char A1 = PINA & 0x02;
    unsigned char A2 = PINA & 0x04;
    unsigned char A7 = PINA & 0x80;

    unsigned char tempB = 0x00;

    switch(state) {
        case Start:
            state = Locked;
            break;

        case Locked:
            state = A2 ? LHash_Press : Locked;
            break;

        case LHash_Press:
            if (tempA == 0)
                state = LHash_Release;
            else if (A2)
                state = LHash_Press;
            else if (A1 || A0)
                state = Locked;
            break;

        case LHash_Release:
            if (tempA == 0)
                state = LHash_Release;
            else if (A1)
                state = Unlocked;
            else if (A2 || A0)
                state = Locked;
            break;

        case Unlocked:
            if (A7)
                state = Locked;
            else if (A2)
                state = UHash_Press;
            else
                state = Unlocked;
            break;

        case UHash_Press:
            if (tempA == 0)
                state = UHash_Release;
            else if (A2)
                state = UHash_Press;
            else if (A1 || A0)
                state = Unlocked;
            break;

        case UHash_Release:
            if (tempA == 0)
                state = UHash_Release;
            else if (A1)
                state = Locked;
            else if (A2 || A0)
                state = Unlocked;
            break;

        default:
            state = Locked;
            break;
    }

    switch(state) {
        case Locked:
            tempB = 0;
            break;

        case Unlocked:
            if (tempB == 0)
                tempB = 1;
            else
                tempB = 0;
            break;

        default:
            tempB = 0;
            break;
    }

    PORTB = tempB;

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
    while (1) {
        Tick();
    }
    return 1;
}
