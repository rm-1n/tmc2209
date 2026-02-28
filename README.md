## Trinamic driver library

21-08-10: Refactored to support ganged motors, some API changes.

Supports TMC2130, TMC2209, TMC2240, TMC2660 and TMC5160 drivers.

Written in plain C, processor agnostic. Processor specific low-level communications layer has to be added by user.

This library is used by some [grblHAL](https://github.com/grblHAL) drivers and examples of low-level communications layers and a [higher level](https://github.com/grblHAL/Plugins_motor) configuration/reporting layer implementation can be found there.

A [SPI <> I2C](https://github.com/terjeio/Trinamic_TMC2130_I2C_SPI_Bridge) bridge implemented on a TI MSP430G2553 processor that may be used for systems with limited IO capabilities.

---
2026-02-28: added support for single pin UART based on raspberry pi pico PIO. 
All added functionality based on raspberrypi example's UART pio implementation (plus some State Machine switching).

Usage: 
0. ADD a pullup to 3.3V for your single wire UART pin
1. Add this project as a git submodule
2. Modify your CMakeLists.txt
```
add_subdirectory(picoTMC2209)
target_link_libraries(project-name
    PRIVATE
        # your other libraries
        pico_tmc2209
        hardware_pio
)
```

3. Modify your code
```
# incomplete example, only focused on TMC2209 related calls
#include "tmc2209.h"
#include "pico-tmc2209.h"

int main(){

    // init the single wire uart
    tmc_uart_init(TMC2209_SERIAL_PIN);
    // Basic velocity setpoint
    static TMC2209_t tmc; // keep in static storage
    TMC2209_SetDefaults(&tmc);
    tmc.config.motor.id = 0;
    tmc.config.motor.address = 0; // typical single-drop / default address
    if (TMC2209_Init(&tmc)) {
         printf("TMC2209 init success\n");
    } else {
        printf("TMC2209 init failed\n");
    }

    // make sure enable is low
    gpio_init(TMC2209_EN_PIN);
    gpio_set_dir(TMC2209_EN_PIN, GPIO_OUT); 
    gpio_put(TMC2209_EN_PIN, false); // enable the driver (active low)

    int speed = 42;

    while (1) {
        uint32_t packed = ((uint32_t)speed) & 0x00ffffffu; // 24-bit two's-complement
        tmc.vactual.reg.value = packed;
        TMC2209_WriteRegister(&tmc, (TMC2209_datagram_t *)&tmc.vactual);
        sleep_ms(100);
    }
}
```