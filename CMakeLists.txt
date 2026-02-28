add_library(pico_tmc2209 INTERFACE)

target_sources(pico_tmc2209 INTERFACE
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

target_include_directories(pico_tmc2209 INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/pio-uart
)

# add pio based single wire uart
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/pio-uart)
target_link_libraries(pico_tmc2209 INTERFACE pio_sw_uart)
