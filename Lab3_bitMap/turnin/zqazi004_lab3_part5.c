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
    DDRB = 0xFE; PORTB = 0x01;
    DDRD = 0x00; PORTD = 0xFF;

    unsigned char tempB = 0x00;
    unsigned char tempD = 0x00;
    unsigned char total_weight = 0x00;

    /* Insert your solution below */
    while (1) {
        tempD = PIND;
        tempB = PINB & 0x01;

        total_weight = (tempD << 1) | tempB;
        
        if (total_weight >= 0x46)
            PORTB = 0x02;
        else if (total_weight > 0x05 && total_weight < 0x46)
            PORTB = 0x04;
        else 
            PORTB = 0x00;
    }
    return 1;
}
