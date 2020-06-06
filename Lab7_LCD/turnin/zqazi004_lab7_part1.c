#include <avr/io.h>
#include "../header/io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char tempA = 0x00;
unsigned char tempB = 0x07;
unsigned char count = 0x00;
//unsigned char score = 0x00;


volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;


enum States{Start, Reset, Add, Sub, Wait} state;

void TimerOn(){
    TCCR1B = 0x0B;
    OCR1A = 125;
    TIMSK1 = 0x02;
    TCNT1 = 0;
    _avr_timer_cntcurr = _avr_timer_M;
    SREG |= 0x80;
}

void TimerOff(){
    TCCR1B = 0x00;
}

void TimerISR(){
    TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
    _avr_timer_cntcurr--;
    if(_avr_timer_cntcurr == 0){
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M){
    _avr_timer_M = M;
    _avr_timer_cntcurr = _avr_timer_M;
}


void Tick(){
    switch(state){
        case Start:
            state = Wait;
            break;
        case Reset:
            state = Wait;
            break;
        case Add:
            if(tempA == 0x01){
                state = Add;
            }
            else if(tempA == 0x03){
                state = Reset;
            }
            else if(tempA == 0x00){
                state = Wait;
            }
            break;
        case Sub:
            if(tempA == 0x02){
                state = Sub;
            }
            else if(tempA == 0x03){
                state = Reset;
            }
            else if(tempA == 0x00){
                state = Wait;
            }
            break;
        case Wait:
            if(tempA == 0x00){
                state = Wait;
            }
            else if(tempA == 0x03){
                state = Reset;
            }
            else if(tempA == 0x01){
                count = 0x0A;
                state = Add;
            }
            else if(tempA == 0x02){
                count = 0x0A;
                state = Sub;
            }
            else {
                state = Wait;
            }
            break;
    }
    switch(state){
        case Start:
            tempB = 0x07;;
            break;

        case Reset:
            count = 0x00;
            tempB = 0x00;
            break;

        case Add:
            if((tempB < 0x09) && (count == 0x0A)){
                tempB = tempB + 0x01;
                count = 0x00;
            }
            count = count + 0x01;
            break;

        case Sub:
            if((tempB > 0x00) && (count == 0x0A)){
                tempB = tempB - 0x01;
                count = 0x00;
            }
            count = count + 0x01;
            break;

        case Wait:
            count = 10;
            break;

        default:
            break;
    }

}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xFF; PORTC = 0x00;
    DDRB = 0xFF; PORTB = 0x07;
    DDRD = 0xFF; PORTD = 0x00;
    DDRA = 0x00; PORTA = 0xFF;

    LCD_init();

    TimerSet(100);
    TimerOn();
    state = Start;
    count = 0x0A;

    /* Insert your solution below */
    while (1) {
        tempA = ~(PINA);
        LCD_Cursor(1);
        Tick();
        PORTB = tempB;
        LCD_WriteData(tempB + '0');

        while(!TimerFlag);
        TimerFlag = 0;
    }
    return 1;
}
