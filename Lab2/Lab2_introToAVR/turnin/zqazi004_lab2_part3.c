/*	Author: Zubair Qazi
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #2  Exercise #3
 *	Exercise Description: Parking Space Sensor
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
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
    
    unsigned char A0 = 0x00; // Temporary variable to hold the value of A
    unsigned char A1 = 0x00;
    unsigned char A2 = 0x00;
    unsigned char A3 = 0x00;

    unsigned char cntavail = 0x00; // Number of Available Spaces

    while(1) {
        // 1) Read input
        
        A0 = PINA & 0x01;
        A1 = PINA & 0x02;
        A2 = PINA & 0x04;
        A3 = PINA & 0x08;

        // 2) Perform computation
        
        // 0000
        if (A0 == 0x00 && A1 == 0x00 && A2 == 0x00 && A3 == 0x00) {
            cntavail = 0x04;
        }

        // 1111
        else if (A0 == 0x01 && A1 == 0x02 && A2 == 0x04 && A3 == 0x08) {
            // Set PC7 to 1
            cntavail = 0x80;
        }

        // 1000 or 0100 or 0010 or 0001
        else if ( (A0 == 0x01 && A1 == 0x00 && A2 == 0x00 && A3 == 0x00) ||
                (A0 == 0x00 && A1 == 0x02 && A2 == 0x00 && A3 == 0x00) ||
                (A0 == 0x00 && A1 == 0x00 && A2 == 0x04 && A3 == 0x00) ||
                (A0 == 0x00 && A1 == 0x00 && A2 == 0x00 && A3 == 0x08) ) 
        {
            cntavail = 0x03;
        }

        // 1001 or 1010 or 1100 or 0101 or 0011 or 0110
        else if ( (A0 == 0x01 && A1 == 0x00 && A2 == 0x00 && A3 == 0x08) ||
                (A0 == 0x01 && A1 == 0x00 && A2 == 0x04 && A3 == 0x00) ||
                (A0 == 0x01 && A1 == 0x02 && A2 == 0x00 && A3 == 0x00) ||
                (A0 == 0x00 && A1 == 0x02 && A2 == 0x00 && A3 == 0x08) ||
                (A0 == 0x00 && A1 == 0x00 && A2 == 0x04 && A3 == 0x08) ||
                (A0 == 0x00 && A1 == 0x02 && A2 == 0x04 && A3 == 0x00) )
        {
            cntavail = 0x02;
        }

        // 1110 or 1101 or 1011 or 0111 
        else if ( (A0 == 0x01 && A1 == 0x02 && A2 == 0x04 && A3 == 0x00) ||
                (A0 == 0x01 && A1 == 0x02 && A2 == 0x00 && A3 == 0x08) ||
                (A0 == 0x01 && A1 == 0x00 && A2 == 0x04 && A3 == 0x08) ||
                (A0 == 0x00 && A1 == 0x02 && A2 == 0x04 && A3 == 0x08) ) 
        {
            cntavail = 0x01;
        }
        
        // 3) Write output
        
        PORTC = cntavail;
    }
    return 0;

}
