#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "dht20_handler.h"


// #include "src/calc.h"

#define TEMP_ADC_PIN 26

int main() {
    stdio_init_all();

    sleep_ms(4000);

    printf("Hello world\n");

    adc_init();
    adc_gpio_init(TEMP_ADC_PIN);
    adc_select_input(0);

    dht20_setupDHT();


    // gpio_init(PICO_DEFAULT_LED_PIN);
    // gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);
    unsigned int uptime = 0;

    float dht_temp, dht_hum;
    while(1) {
        // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);   //8*10^-4 3.3 / 4096
        uint16_t result = adc_read(); // 0x430 = 1072
        float c_per_mv = 19.5f;
        float tmp = ((result * conversion_factor) * 1000-400) / c_per_mv;
        printf("Temp: %f'c Raw value: %d, voltage: %f V\n", tmp, result, result * conversion_factor);

        if (dht20_read(&dht_temp, &dht_hum) != 0) {
            printf("DHT reading failed.\n");
        }else {
            printf("--- DHT Measurements\n");
            printf("--- Temperature: %5.2f C°", dht_temp);
            printf("--- Humidity: %5.2f \%RH\n", dht_hum);
        }
        // printf("Uptime: %d\n", uptime);
        uptime++;
        sleep_ms(4000);
    }
}