/*	Author: Zubair Qazi
 *  Partner(s) Name: 
 *	Lab Section:024
 *	Assignment: Lab #2  Exercise #1
 *	Exercise Description: Garage Door Sensor
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
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
    unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
    while(1) {
        // 1) Read input
        tmpA0 = PINA & 0x01;
        tmpA1 = PINB & 0x02;
        // 2) Perform computation

        if (tmpA0 == 0x01 && tmpA1 == 0x00) { // True if PA0 is 1 and PA1 is 0
            tmpB = (tmpB & 0xFE) | 0x01; // Sets tmpB to bbbbbbb1
                             // (clear rightmost 1 bits, then set to 1)
        } else {
            tmpB = (tmpB & 0xFE) | 0x00; // Sets tmpB to bbbbbbb0
                             // (clear rightmost 1 bits, then set to 0)
        }    
        // 3) Write output
        PORTB = tmpB;    
    }
    return 0;

}
