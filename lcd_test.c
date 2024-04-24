#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>

#include "lcd_handler.h"

// #define MESSAGE_LEN = 16
// struct message {
//     char msg[MESSAGE_LEN+1];
// }

#define MESSAGES_COUNT 4
int main() {
    stdio_init_all();

    sleep_ms(5000);
    printf("lcd_handler test\n");

    lcdhandler_init();

    // sleep_ms(1000);
    char messages[MESSAGES_COUNT][20] = {
        "Hello World!",
        "Second Message",
        "Temp: 24.0",
        "Hum: 50.0",
    };
    printf("Write message 'Hello World!'\n");
    char msgHello[] = "Hello World!";
    lcdhandler_write_line(msgHello, 0);

    printf("Write to line 1\n");
    for (int i = 0; i < MESSAGES_COUNT; i++) {
        sleep_ms(1000);
        char *msg = messages[i];
        printf("Write message: '%s'\n", msg);
        lcdhandler_write_line(msg, 0);
    }

    sleep_ms(1000);
    printf("Write to line 2\n");
    for (int i = 0; i < MESSAGES_COUNT; i++) {
        sleep_ms(1000);
        char *msg = messages[i];
        printf("Write message: '%s'\n", msg);
        lcdhandler_write_line(msg, 1);
    }

    float temps[] = {24.0f, 28.0f, 18.0f, 22.0f, 20.0f, 22.0f, 29.0f, 17.0f,24.0f, 24.0f, 24.0f};
    float hums[] = {50.0f, 40.0f, 50.f, 50.f, 45.f, 30.f, 10.f, 50.f,50.f,50.f};
    // float temp = 24.f;
    // float hum = 1.f;
    for (int i = 0; i < 10; i++) {
        sleep_ms(1000);
        char msg1[20];
        char msg2[20];
        sprintf(msg1, "Temp: %5.2f C°", temps[i]);
        sprintf(msg2, "HUM: %5.2f \%RH", hums[i]);
        lcdhandler_write_line(msg1, 0);
        lcdhandler_write_line(msg2, 1);
    }

    sleep_ms(2000);
    lcdhandler_clear();

    printf("Test done\n");
    while(1) {
        //Nothing to do, keep usb alive
        tight_loop_contents();
        sleep_ms(1000);
    }
    return 0;
}