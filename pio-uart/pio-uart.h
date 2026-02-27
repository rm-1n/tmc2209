// Minimal PIO-UART shim (uses existing onewire primitives for now)
#ifndef PIO_UART_H
#define PIO_UART_H

#include <stdint.h>

// Initialise pio-uart on given GPIO (shim -> initialises onewire)
void pio_uart_init(uint32_t gpio_pin);

// Send len bytes from buf
void pio_uart_send(const uint8_t *buf, int len);

// Receive len bytes into buf (blocking)
void pio_uart_recv(uint8_t *buf, int len);
#endif
