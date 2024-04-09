#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"


// #include "src/calc.h"

#define TEMP_ADC_PIN 26

int main() {
    stdio_init_all();

    printf("Hello world\n");

    adc_init();
    adc_gpio_init(TEMP_ADC_PIN);


    // gpio_init(PICO_DEFAULT_LED_PIN);
    // gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);
    unsigned int uptime = 0;

    adc_select_input(0);
    while(true) {
        // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);   //8*10^-4 3.3 / 4096
        uint16_t result = adc_read(); // 0x430 = 1072
        float c_per_mv = 19.5f;
        float tmp = ((result * conversion_factor) * 1000-400) / c_per_mv;
        printf("Temp: %f'c Raw value: %d, voltage: %f V\n", tmp, result, result * conversion_factor);
        // printf("Uptime: %d\n", uptime);
        uptime++;
        sleep_ms(1000);
    }
}