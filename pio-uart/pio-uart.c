// PIO-based UART shim using generated PIO programs (uart_tx.pio, uart_rx.pio)

#include "pio-uart.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "uart_tx.pio.h"
#include "uart_rx.pio.h"

// Set up the state machine we're going to use to receive them.
PIO rx_pio;
uint rx_sm;
uint rx_offset;
PIO tx_pio;
uint tx_sm;
uint tx_offset;
static uint32_t uart_gpio = 0;
static bool uart_inited = false;


void pio_uart_init(uint32_t gpio_pin)
{
    if (uart_inited) return;
    uart_gpio = gpio_pin;

    // Use pio0 for now; claim SMs and add programs
    PIO p = pio0;

    int off_tx = pio_add_program(p, &uart_tx_program);
    tx_sm = pio_claim_unused_sm(p, true);
    tx_pio = p;
    tx_offset = off_tx;
    uart_tx_program_init(tx_pio, tx_sm, tx_offset, gpio_pin, SERIAL_BAUD);

    int off_rx = pio_add_program(p, &uart_rx_mini_program);
    rx_sm = pio_claim_unused_sm(p, true);
    rx_pio = p;
    rx_offset = off_rx;
    uart_rx_mini_program_init(rx_pio, rx_sm, rx_offset, gpio_pin, SERIAL_BAUD);

    uart_inited = true;
}

void pio_uart_send(const uint8_t *buf, int len)
{
    if (!uart_inited) return;
    // switch to TX: disable RX SM, set pin as output and enable TX SM
    pio_sm_set_enabled(rx_pio, rx_sm, false);
    pio_sm_set_consecutive_pindirs(tx_pio, tx_sm, uart_gpio, 1, true);
    pio_sm_set_enabled(tx_pio, tx_sm, true);

    for (int i = 0; i < len; ++i) {
        uart_tx_program_putc(tx_pio, tx_sm, (char)buf[i]);
    }

    // wait for TX FIFO to drain
    while (pio_sm_get_tx_fifo_level(tx_pio, tx_sm) > 0) {
        tight_loop_contents();
    }
    sleep_us(WAIT_TIME_PER_FRAME_US);
}

int pio_uart_recv(uint8_t *buf, int len, uint32_t timeout_us)
{
    int read = 0;
    if (!uart_inited) return read;
    // ensure RX SM enabled and TX disabled
    pio_sm_set_enabled(tx_pio, tx_sm, false);
    pio_sm_set_consecutive_pindirs(rx_pio, rx_sm, uart_gpio, 1, false);
    pio_sm_set_enabled(rx_pio, rx_sm, true);

    for (int i = 0; i < len; ++i) {
        uint32_t start = time_us_32();
        while (pio_sm_is_rx_fifo_empty(rx_pio, rx_sm)) {
            if ((time_us_32() - start) >= timeout_us) {
                return read; // timeout, return bytes read so far
            }
            tight_loop_contents();
        }
        char c = uart_rx_program_getc(rx_pio, rx_sm);
        buf[i] = (uint8_t)(c & 0xffu);
        read++;
    }
    return read;
}