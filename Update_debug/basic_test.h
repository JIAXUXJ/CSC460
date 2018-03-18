#include <stdint.h>
#include "os.h"
#include "active.h"
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/delay.h>

void Pong() {
    for(;;) {
        PORTB &= ~(1 << PORTB7);
         _delay_ms(500);
        PORTB |= 1 << PORTB7;
    }
}

void Ping() {
    for (;;) {
        PORTB &= ~(1 << PORTB6);
         _delay_ms(500);
        PORTB |= 1 << PORTB6;
    }
}

void a_main(void) {
    Task_Create_System(Pong, 0);
//    Task_Create_RR(Ping, 0);
    /* Task_Create_RR(Pong, 0); */
}

