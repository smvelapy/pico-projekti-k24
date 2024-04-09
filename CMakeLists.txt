cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
include(pico-sdk/pico_sdk_init.cmake)
include(pico-sdk/external/pico_sdk_import.cmake)

project(picoapp C CXX ASM)

pico_sdk_init()

add_compile_options(-Wall
	-Wno-format
	-Wno-unused-function
	-Wno-maybe-uninitialized
	)

add_executable(picoapp
    main.c
	# hello.c
)

# add_subdirectory("keypad-switch")
# target_include_directories(keypad_switch PUBLIC "keypad-switch")
# target_link_directories(keypad_switch PUBLIC "keypad-switch")

target_link_libraries(picoapp pico_stdlib hardware_adc)

# add_subdirectory("grove-lcd-pico")
# target_include_directories(rgb_lcd PUBLIC "grove-lcd-pico")
# target_link_directories(rgb_lcd PUBLIC "grove-lcd-pico")
# target_link_libraries(picoapp pico_stdlib hardware_i2c pico_binary_info rgb_lcd)




pico_enable_stdio_usb(picoapp 1)
pico_enable_stdio_uart(picoapp 0)

pico_add_extra_outputs(picoapp)
