/*	Author: Zubair Qazi
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #2  Exercise #4
 *	Exercise Description: Ride Weight Sensor
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {

    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0x00; PORTC = 0xFF;
    DDRD = 0xFF; PORTD = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
    
    unsigned short total_weight = 0x00; //Total weight of A, B, and C
    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;

    unsigned char ret = 0x00;

    while(1) {
        // 1) Read input
        tmpA = PINA;
        tmpB = PINB;
        tmpC = PINC;

        // 2) Perform computation
        
        total_weight = tmpA + tmpB + tmpC;

        if (total_weight > 0x8C) {
            ret = ret |  0x01;
        }
        
        if (tmpA > tmpC) {
            if (tmpA - tmpC > 0x50)
                ret = ret | 0x02;
        }
        else {
            if (tmpC - tmpA > 0x50)
                ret = ret | 0x02;
        }

        ret = (total_weight & 0xFC) | ret;
        
        // 3) Write output
        
        PORTD = ret;
    }
    return 0;

}
