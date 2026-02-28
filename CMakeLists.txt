add_library(tmc2209 INTERFACE)

target_sources(tmc2209 INTERFACE
 ${CMAKE_CURRENT_LIST_DIR}/common.c
 ${CMAKE_CURRENT_LIST_DIR}/tmc_interface.c
#  ${CMAKE_CURRENT_LIST_DIR}/tmc2130.c
#  ${CMAKE_CURRENT_LIST_DIR}/tmc2130hal.c
 ${CMAKE_CURRENT_LIST_DIR}/tmc2209.c
 ${CMAKE_CURRENT_LIST_DIR}/tmc2209hal.c
 ${CMAKE_CURRENT_LIST_DIR}/pico-tmc2209.c
#  ${CMAKE_CURRENT_LIST_DIR}/tmc2240.c
#  ${CMAKE_CURRENT_LIST_DIR}/tmc2240hal.c
#  ${CMAKE_CURRENT_LIST_DIR}/tmc2660.c
#  ${CMAKE_CURRENT_LIST_DIR}/tmc2660hal.c
#  ${CMAKE_CURRENT_LIST_DIR}/tmc5160.c
#  ${CMAKE_CURRENT_LIST_DIR}/tmc5160hal.c
)

target_include_directories(tmc2209 INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/pio-uart
)

# Using pio-uart shim (bit-banged or PIO implementation)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/pio-uart)
target_link_libraries(tmc2209 INTERFACE pio_uart_shim)
