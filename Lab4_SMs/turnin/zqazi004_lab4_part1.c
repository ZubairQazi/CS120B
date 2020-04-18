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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//#include "RIMS.h"
enum States {Start, OFF_RELEASE, OFF_PRESS, ON_RELEASE, ON_PRESS} state;

void Tick() {
    
    unsigned char A0 = PINA & 0x01;

    switch(state) {
        case Start:
            state = OFF_RELEASE;
            break;
        
        case OFF_RELEASE:
            state = A0 ? ON_PRESS : OFF_RELEASE;
            break;

        case ON_PRESS:
            state = A0 ? ON_PRESS : ON_RELEASE;
            break;

        case ON_RELEASE:
            state = A0 ? OFF_PRESS : ON_RELEASE;
            break;

        case OFF_PRESS:
            state = A0 ? OFF_PRESS : OFF_RELEASE;
            break;

        default:
            state = OFF_RELEASE;
            break;
    }

    switch(state) {
        case OFF_RELEASE:
            PORTB = 0x01;
            break;
        
        case ON_PRESS:
            PORTB = 0x02;
            break;

        case ON_RELEASE:
            PORTB = 0x02;
            break;

        case OFF_PRESS:
            PORTB = 0x01;
            break;

        default:
            PORTB = 0x00;
            break;
    }

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
