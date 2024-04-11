#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
// #include "pico/binary_info.h"
#define POTENT_OUT_PIN 28
int main() {
    stdio_init_all();

    sleep_ms(4000);
    printf("Potentiometer test\n");

    adc_init();
    adc_gpio_init(POTENT_OUT_PIN);
    adc_select_input(2);

    while(1) {
        uint16_t result = adc_read() * 100 / (1 << 12);
        printf("Result: %d\n",result);
        sleep_ms(1000);
    }

}