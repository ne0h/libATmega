#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <usart.hpp>

#define LED_PORT PORTB
#define LED_DDRx DDRB
#define LED_PIN  PB5

static volatile uint8_t state = 0;
static AsyncUSART u;

// Interrupt when a symbole is received
ISR(USART_RX_vect) {
    // Send received symbole back
    u.send(UDR0);

    // Toggle LED state
    state = state ? 0 : 1;
    LED_PORT = (state<<LED_PIN);
}

int main() {

    // Set LED pin to output
	LED_DDRx |= (1<<LED_PIN);
    LED_PORT = 0x00;

    // Activate interrupts.
    sei();

    while (true) {}
	return 0;
}
