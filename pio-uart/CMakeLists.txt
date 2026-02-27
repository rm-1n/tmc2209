add_library(pio_uart_shim STATIC pio-uart.c)
target_include_directories(pio_uart_shim PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(pio_uart_shim PRIVATE ${CMAKE_CURRENT_LIST_DIR})

set_target_properties(pio_uart_shim PROPERTIES PREFIX "")

target_link_libraries(pio_uart_shim PRIVATE pico_stdlib hardware_pio)

# Generate PIO headers for uart tx/rx
pico_generate_pio_header(pio_uart_shim ${CMAKE_CURRENT_LIST_DIR}/uart_tx.pio)
pico_generate_pio_header(pio_uart_shim ${CMAKE_CURRENT_LIST_DIR}/uart_rx.pio)
