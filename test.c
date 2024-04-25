#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"


bool repeating_timer_callback(struct repeating_timer *t) {
    // printf("Repeat at %lld\n", time_us_64());
    printf("Get settings\n");
    return true;
}
bool repeating_timer2_callback(struct repeating_timer *t) {
    // printf("Repeat at %lld\n", time_us_64());
    printf("-- POST DATA ---");
    return true;
}

int main()
{
    stdio_init_all();

    sleep_ms(5000);
    printf("Timer test\n");

    struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

    struct repeating_timer timer2;
    add_repeating_timer_ms(5000, repeating_timer2_callback, NULL, &timer2);

    while(1) {
        tight_loop_contents();
    }

    return 0;
}
