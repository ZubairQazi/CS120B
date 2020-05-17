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

enum States{Start, Wait, Add, Add_Release,  Sub, Sub_Release, Off, Off_Release, On} state;

double freqs[] = {261.63, 293.66, 329.64, 349.23, 392, 440, 493.88, 523.25};
unsigned char i = 4;

double frequency = 0;

void set_PWM(double frequency) {
    static double current_frequency;

    if (frequency != current_frequency) {
        if(!frequency)
            TCCR3B &= 0x08;
        else
            TCCR3B |= 0x03;

        if(frequency < 0.954)
            OCR3A = 0xFFFF;
        else if (frequency > 31250)
            OCR3A = 0x0000;
        else
            OCR3A = (short) (8000000 / (128 * frequency)) - 1;
        
        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

void Tick() {
    unsigned char A0 = ~PINA & 0x01;
    unsigned char A1 = ~PINA & 0x02;
    unsigned char A2 = ~PINA & 0x04;

    switch(state) {
        case Start:
            state = Off;
            break;

        case Wait:
            if (A0)
                state = Add;
            else if (A1)
                state = Sub;
            else if (A2)
                state = Off_Release;
            else
                state = Wait;
            break;

        case Off:
            state = A2 ? On : Off;
            break;

        case Off_Release:
            state = A2 ? Off_Release : Off;
            break;

        case On:
            state = A2 ? On : Wait;
            break;

        case Add:
            state = Add_Release;
            break;

        case Add_Release:
            state = A0 ? Add_Release : Wait;
            break;

        case Sub:
            state = Sub_Release;
            break;

        case Sub_Release:
            state = A1 ? Sub_Release : Wait;
            break;

        default:
            state = Start;
            break;
    }

    switch(state) {
        case Add:
            if (i < 7)
                i++;
            break;

        case Sub:
            if (i > 0)
                i--;
            break;

        case Off:
            PWM_off();
            break;

        case On:
            PWM_on();
            i = 4;
            break;

        default:
            break;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
    PWM_off();
    state = Start; 
    
    while (1) {
        //set_PWM(261.63);
        Tick();
        set_PWM(freqs[i]);
    }
    return 1;
}
