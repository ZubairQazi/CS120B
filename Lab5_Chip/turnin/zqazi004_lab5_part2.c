// https://drive.google.com/drive/folders/1s9MdbH1g7w5wyXiyYg8XfnSyaMMarmXs?usp=sharing


/*	Author: Zubair
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #5  Exercise #2
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
enum States {Start, Init, Add_Press, Add_Release, Sub_Press, Sub_Release, Reset} state;

void Tick() {
    
    unsigned char tempA = ~PINA;
    unsigned char tempC = PORTC;

    switch(state) {
        case Start:
            state = Init;
            break;

        case Init:
            if (tempA == 1)
                state = Add_Press;
            else if (tempA == 2) 
                state = Sub_Press;
            else if (tempA == 3)
                state = Reset;
            break;

        case Add_Press:
            state = Add_Release;
            break;

        case Add_Release:
            if (tempA == 0)
                state = Init;
            else if (tempA == 1)
                state = Add_Release;
            else if (tempA == 3)
                state = Reset;
            break;

        case Sub_Press:
            state = Sub_Release;
            break;

        case Sub_Release:
            if (tempA == 0)
                state = Init;
            else if (tempA == 2)
                state = Sub_Release;
            else if (tempA == 3)
                state = Reset;
            break;

        case Reset:
            if (!tempA)
                state = Init;
            else
                state = Reset;
            break;

        default:
            state = Init;
            break;
    }

    switch(state) {
        case Add_Press:
            if (tempC < 9)
                tempC = tempC + 1;
            break;

        case Sub_Press:
            if (tempC > 0)
                tempC = tempC - 1;
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
