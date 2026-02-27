/* Pico HAL for TMC UART using the onewire PIO driver */

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "common.h"
#include "pio-uart/pio-uart.h"
#include "hardware/pio.h"

static bool tmc_ow_inited = false;

// Initialise the TMC one-wire interface on the given GPIO pin. This must be
// called from board code (constructor/init) with the desired serial pin.
void tmc_uart_init(uint gpio_pin)
{
    if (tmc_ow_inited) return;

    // initialise PIO-UART shim which currently wraps the onewire primitives
    pio_uart_init(gpio_pin);
    tmc_ow_inited = true;
    printf("tmc_uart_init: pio-uart initialized on gpio=%d\n", gpio_pin);
}

void tmc_uart_write (trinamic_motor_t driver, TMC_uart_write_datagram_t *datagram)
{
    (void) driver;
    if (!tmc_ow_inited) return; // must call tmc_uart_init() first

    printf("tmc_uart_write: sending %d bytes (datagram):\n", (int)sizeof(datagram->data));
    for (int i = 0; i < (int)sizeof(datagram->data); ++i) printf(" %02x", datagram->data[i]);
    printf("\n");

    // Send entire datagram via PIO-UART shim
    pio_uart_send(datagram->data, (int)sizeof(datagram->data));
}

TMC_uart_write_datagram_t *tmc_uart_read (trinamic_motor_t driver, TMC_uart_read_datagram_t *datagram)
{
    static TMC_uart_write_datagram_t resp;
    (void) driver;

    if (!tmc_ow_inited) return NULL; // must call tmc_uart_init() first

    // send datagram, then receive response
    pio_uart_send(datagram->data, (int)sizeof(datagram->data));
    // short delay per SENDDELAY guidance
    sleep_us(800);
    pio_uart_recv(resp.data, (int)sizeof(resp.data));

    printf("tmc_uart_read: resp:");
    for (int i = 0; i < (int)sizeof(resp.data); ++i) printf(" %02x", resp.data[i]);
    printf("\n");
    return &resp;
}
