#include <stdint.h>

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SIGNAL_DDR DDRB
#define SIGNAL_PORT PORTB
#define SIGNAL_PIN 5
#define BAUD 9600
#define SEND_REPEATS 4

#define DELAY_SHORT 280
#define DELAY_LONG 1350
#define DELAY_SYNC 2780


void write_sync(void)
{
    SIGNAL_PORT |= (1 << SIGNAL_PIN);
    _delay_us(275);
    SIGNAL_PORT &= ~(1 << SIGNAL_PIN);
    _delay_us(2675);
}

void write_0(void)
{
    SIGNAL_PORT |= (1 << SIGNAL_PIN);
    _delay_us(275);
    SIGNAL_PORT &= ~(1 << SIGNAL_PIN);
    _delay_us(275);
    SIGNAL_PORT |= (1 << SIGNAL_PIN);
    _delay_us(275);
    SIGNAL_PORT &= ~(1 << SIGNAL_PIN);
    _delay_us(1225);
}

void write_1(void)
{
    SIGNAL_PORT |= (1 << SIGNAL_PIN);
    _delay_us(275);
    SIGNAL_PORT &= ~(1 << SIGNAL_PIN);
    _delay_us(1225);
    SIGNAL_PORT |= (1 << SIGNAL_PIN);
    _delay_us(275);
    SIGNAL_PORT &= ~(1 << SIGNAL_PIN);
    _delay_us(275);
}

void write_code(uint32_t code)
{
    write_sync();
    for (int i = 0; i < 32; i++) {
        if (code & (1ul << 31)) {
            write_1();
        } else {
            write_0();
        }
        code <<= 1;
    }
    write_sync();
}

uint8_t uart_receive(void)
{
    while (!(UCSR0A & (1 << RXC0))) {}
    return UDR0;
}

void uart_flush(void)
{
    unsigned char discard;
    while (UCSR0A & (1 << RXC0)) {
        discard = UDR0;
    }
}

int main(void)
{
    SIGNAL_DDR |= (1 << SIGNAL_PIN);

    // UART setup
    UBRR0H = 0;
    UBRR0L = F_CPU / 16 / BAUD - 1; // 9600 baud
    UCSR0B = 1 << RXEN0;
    UCSR0C = 3 << UCSZ00; // 8 data bits

    while (1) {
        uint32_t code;
        unsigned char* code_bytes = (void*)&code;
        code_bytes[0] = uart_receive();
        code_bytes[1] = uart_receive();
        code_bytes[2] = uart_receive();
        code_bytes[3] = uart_receive();
        uart_flush();

        for (int i = 0; i < SEND_REPEATS; i++) {
            write_code(code);
            _delay_us(7125);
        }
    }

    return 0;
}
