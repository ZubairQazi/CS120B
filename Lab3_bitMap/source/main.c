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
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char tempA = 0x00;
    unsigned char tempB = 0x00;
    unsigned char tempC = 0x00;

    /* Insert your solution below */
    while (1) {
        tempA = PINA;
        tempB = PINB;
        tempC = 0x00;

        for (unsigned char i = 0; i < 8; i++) {
            if ((tempA >> i) & 0x01)
                tempC++;
            if ((tempB >> i) & 0x01)
                tempC++;
        }

        PORTC = tempC;
    }
    return 1;
}
