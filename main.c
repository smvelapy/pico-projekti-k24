#include <stdio.h>
#include "pico/stdlib.h"


// #include "src/calc.h"

// #define LED_PIN = 15;
int main() {
    const unsigned int LED_PIN = 16;//15;
    stdio_init_all();
    printf("Hello world\n");
    // gpio_init(PICO_DEFAULT_LED_PIN);
    // gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);
    unsigned int uptime = 0;
    while(true) {
        printf("Uptime: %d\n", uptime);
        // if (uptime < 10) {
        //     gpio_put(PICO_DEFAULT_LED_PIN, 1);
        //     sleep_ms(500);
        //     gpio_put(PICO_DEFAULT_LED_PIN, 0);
        //     sleep_ms(500);
        // }else if (uptime == 20) {
        //     printf("Turning LED on.\n");
        //     gpio_put(LED_PIN, 1);
        //     // sleep_ms(500);
        //     // gpio_put(LED_PIN, 0);
        //     // sleep_ms(500);
        // }else {
        //     // printf("Ending program.\n");
        //     sleep_ms(1000);
        //     // break;
        // }
        uptime++;
        sleep_ms(1000);
    }
}