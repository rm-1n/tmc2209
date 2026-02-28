// Pico-specific TMC2209 HAL helpers
#ifndef PICO_TMC2209_H
#define PICO_TMC2209_H

#include <stdint.h>
#include "pio-uart.h"
#define SENDDELAY (BIT_TIME_US * 8) // default 8 bits

// Initialize the TMC UART over the PIO one-wire driver on the specified GPIO
// pin. Call once during board initialization before any `tmc_uart_*` calls.
void tmc_uart_init(uint gpio_pin);

#endif /* PICO_TMC2209_H */

