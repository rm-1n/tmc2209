// Minimal PIO-UART shim (uses existing onewire primitives for now)
#ifndef PIO_UART_H
#define PIO_UART_H

#include <stdint.h>

// when using serial baud 9600 + 19200 a pullup from uart gpio to 3.3V was necessary
#define SERIAL_BAUD 115200
// ensure the last byte finishes transmitting (start + 8 data + stop = 10 bits)
// compute bit time in microseconds and wait one byte time plus margin
#define WAIT_BITS_PER_FRAME 10
#define BIT_TIME_US (1000000 / SERIAL_BAUD)
#define WAIT_TIME_PER_FRAME_US (WAIT_BITS_PER_FRAME * BIT_TIME_US)

// Initialise pio-uart on given GPIO (shim -> initialises onewire)
void pio_uart_init(uint32_t gpio_pin);

// Send len bytes from buf
void pio_uart_send(const uint8_t *buf, int len);

// Receive up to len bytes into buf. Returns number of bytes read (may be 0 on timeout).
// timeout_us: maximum time in microseconds to wait per byte.
int pio_uart_recv(uint8_t *buf, int len, uint32_t timeout_us);
#endif
