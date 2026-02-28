/* Pico HAL for TMC UART using the onewire PIO driver */

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "common.h"
#include "pio-uart/pio-uart.h"
#include "hardware/pio.h"
#include "pico-tmc2209.h"

static bool tmc_uart_inited = false;

// Initialise the TMC single wire uart interface on the given GPIO pin
void tmc_uart_init(uint gpio_pin)
{
    if (tmc_uart_inited) return;

    // initialise PIO-UART
    pio_uart_init(gpio_pin);
    tmc_uart_inited = true;
}

void tmc_uart_write (trinamic_motor_t driver, TMC_uart_write_datagram_t *datagram)
{
    (void) driver;
    if (!tmc_uart_inited) return; // must call tmc_uart_init() first
    // Send entire datagram via PIO-UART
    pio_uart_send(datagram->data, (int)sizeof(datagram->data));
}

TMC_uart_write_datagram_t *tmc_uart_read (trinamic_motor_t driver, TMC_uart_read_datagram_t *datagram)
{
    static TMC_uart_write_datagram_t resp;
    (void) driver;

    if (!tmc_uart_inited) return NULL; // must call tmc_uart_init() first

    // send datagram, then receive response. Fail on CRC error.
    pio_uart_send(datagram->data, (int)sizeof(datagram->data));
    // ensure time for bus transition from host to node
    sleep_us(SENDDELAY);
    // timput of recv used as delay after send
    int got = pio_uart_recv(resp.data, (int)sizeof(resp.data), 2000);
    if (got <= 0) {
        for (int i = 0; i < (int)sizeof(resp.data); ++i) resp.data[i] = 0;
        printf("tmc_uart_read: no response received (got %d bytes)\n", got);
    } else {
        // compute CRC over received length: copy into tmp with zero CRC byte
        uint8_t tmp[got];
        for (int k = 0; k < got; ++k) tmp[k] = resp.data[k];
        tmp[got - 1] = 0;
        tmc_crc8(tmp, (uint8_t)got);
        uint8_t expected_crc = tmp[got - 1];
        if (expected_crc != resp.data[got - 1]) {
            printf("tmc_uart_read: crc mismatch (got 0x%02x expected 0x%02x)\n",
                   resp.data[got - 1], expected_crc);
            for (int i = 0; i < (int)sizeof(resp.data); ++i) resp.data[i] = 0;
        }
    }
    sleep_us(SENDDELAY);
    return &resp;
}
