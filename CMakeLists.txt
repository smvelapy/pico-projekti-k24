cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
include(pico-sdk/pico_sdk_init.cmake)
include(pico-sdk/external/pico_sdk_import.cmake)

set(PICO_BOARD pico_w)


project(picoapp C CXX ASM)

pico_sdk_init()

add_compile_options(-Wall
	-Wno-format
	-Wno-unused-function
	-Wno-maybe-uninitialized
	)



add_executable(picoapp
)

target_sources(picoapp PRIVATE main.c dht20_handler.c web_handler.c lcd_handler.c cJSON.c)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR} )

add_subdirectory("dht20-pico")
target_include_directories(dht20 PUBLIC "dht20-pico")
target_link_directories(dht20 PUBLIC "dht20-pico")

add_subdirectory("grove-lcd-pico")
target_include_directories(rgb_lcd PUBLIC "grove-lcd-pico")
target_link_directories(rgb_lcd PUBLIC "grove-lcd-pico")

target_link_libraries(picoapp pico_stdlib hardware_adc hardware_i2c pico_binary_info dht20 pico_cyw43_arch_lwip_threadsafe_background pico_lwip_http rgb_lcd)



pico_enable_stdio_usb(picoapp 1)
pico_enable_stdio_uart(picoapp 0)

pico_add_extra_outputs(picoapp)
