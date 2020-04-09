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

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char tempA = 0x00;
    unsigned char tempC = 0x00;

    /* Insert your solution below */
    while (1) {
        tempA = PINA;
        tempC = 0x00;

        if (tempA <= 2 && tempA >= 1) 
            tempC = tempC | 0x20;
        else if (tempA <= 4 && tempA >= 3)
            tempC = tempC | 0x30;
        else if (tempA <= 6 && tempA >= 5)
            tempC = tempC | 0x38;
        else if (tempA <= 9 && tempA >= 7)
            tempC = tempC | 0x3C;
        else if (tempA <= 12 && tempA >= 10)
            tempC = tempC | 0x3E;
        else if (tempA <= 15 && tempA >= 13)
            tempC = tempC | 0x3F;

        if (tempA <= 4)
            tempC = tempC | 0x40;

        PORTC = tempC;
    }
    return 1;
}
