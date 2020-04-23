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
enum States {Start, Init, Middle, Left, Right, Reset} state;

void Tick() {
    
    unsigned char tempA = ~PINA;
    unsigned char tempC = PORTC;

    switch(state) {
        case Start:
            state = Init;
            break;

        case Init:
            if (tempA == 0x01)
                state = Middle;
            else
                state = Init;
            break;

        case Middle:
            if (tempA == 0x01)
                state = Left;
            else
                state = Middle;
            break;

        case Left:
            if (tempA == 0x01)
                state = Right;
            else
                state = Left;
            break;

        case Right:
            if (tempA == 0x01)
                state = Init;
            else
                state = Left;
            break;

        case Reset:
            state = Init;
            break;

        default:
            state = Init;
            break;
    }

    switch(state) {
        case Init:
            tempC = 0x3F;
            break;

        case Middle:
            tempC = 0x0C;
            break;

        case Left:
            tempC = 0x03;
            break;

        case Right:
            tempC = 0x30;
            break;
        
        case Reset:
            tempC = 0x00;
            break;

        default:
            break;
    }

    PORTC = tempC;

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    /* Insert your solution below */
    while (1) {
        Tick();
    }
    return 1;
}
