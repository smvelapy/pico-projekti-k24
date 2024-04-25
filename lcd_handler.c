#include "lcd_handler.h"

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include <stdio.h>
#include <string.h>

#include "rgb_lcd.h"


#define LCD_SDA_PIN 4
#define LCD_SCL_PIN 5

#define LINE_LENGTH 16
#define LINES_MAX 2

struct Displaystate disp;
struct Displaystate *disp_ptr = &disp;

void lcdhandler_init()
{
    printf("[lcdhandler] init\n");

    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(LCD_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(LCD_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(LCD_SDA_PIN);
    gpio_pull_up(LCD_SCL_PIN);

    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(LCD_SDA_PIN, LCD_SCL_PIN, GPIO_FUNC_I2C));

    // Set up Grove LCD monitor
    lcd_init(disp_ptr);
    // Use 16 columns, 2 rows
    lcd_begin(disp_ptr, LINE_LENGTH, LINES_MAX);
    // Initialize cursor to start
    lcd_set_cursor(disp_ptr, 0, 0);
}
void lcdhandler_clear() {
    printf("[lcdhandler] clear\n");
    for (uint32_t i = 0; i < LINE_LENGTH; i++)
    {
        lcd_set_cursor(disp_ptr, i, 0);
        lcd_write(disp_ptr, ' ');
        lcd_set_cursor(disp_ptr, i, 1);
        lcd_write(disp_ptr, ' ');
        // sleep_ms(50);
    }
}
int lcdhandler_write_line(const char *msg, uint8_t line_index) {
    int len = strlen(msg);
    printf("[lcdhandler] write msg: '%s' len: %d line_index: %d\n", msg, len, line_index);
    if (len > LINE_LENGTH) {
        printf("[lcdhandler] Warning message length %d max %d, will be cropped.\n", len, LINE_LENGTH);
        // Message too long, abort
        // return -1;
    }
    if (line_index+1 > LINES_MAX) {
        // Line index out of bounds, abort
        return -1;
    }
    char letter = ' ';
    for (uint32_t i = 0; i < LINE_LENGTH; i++) {
        letter = i < len ? msg[i] : ' ';
        lcd_set_cursor(disp_ptr, i, line_index);
        lcd_write(disp_ptr, letter);
    }
    return 0;
}