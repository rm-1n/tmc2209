add_library(pio_sw_uart STATIC pio-uart.c)
target_include_directories(pio_sw_uart PRIVATE ${CMAKE_CURRENT_LIST_DIR}/..)
target_include_directories(pio_sw_uart PRIVATE ${CMAKE_CURRENT_LIST_DIR})

set_target_properties(pio_sw_uart PROPERTIES PREFIX "")

target_link_libraries(pio_sw_uart PRIVATE pico_stdlib hardware_pio)

# Generate PIO headers for uart tx/rx
pico_generate_pio_header(pio_sw_uart ${CMAKE_CURRENT_LIST_DIR}/uart_tx.pio)
pico_generate_pio_header(pio_sw_uart ${CMAKE_CURRENT_LIST_DIR}/uart_rx.pio)
