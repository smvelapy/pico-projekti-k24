#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "dht20-pico/DHT20.h"
// #include "src/calc.h"

#define DHT_I2C_INST i2c0
#define DHT_I2C_SDA 12
#define DHT_I2C_SCL 13
// #define DHT_I2C_SDA 4
// #define DHT_I2C_SCL 5

DHT20 sens;
DHT20 *sens_ptr = &sens;

int setupDHT() {
    printf("Initialize DHT20\n");

    i2c_init(DHT_I2C_INST, 100 * 1000);
    gpio_set_function(DHT_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(DHT_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(DHT_I2C_SDA);
    gpio_pull_up(DHT_I2C_SCL);
    bi_decl(bi_2pins_with_func(DHT_I2C_SDA, DHT_I2C_SCL, GPIO_FUNC_I2C));

    int sensor_ret = DHT20_init(sens_ptr);
    if (sensor_ret != DHT20_OK) {
        printf("DHT20 initialization failed. ret: %d\n", sensor_ret);
        return sensor_ret;
    }

    printf("DHT20 initialized.\n");

    sleep_ms(10);
    return 0;
}
void loop() {
    // gpio_init(PICO_DEFAULT_LED_PIN);
    // gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);
    unsigned int uptime = 0;

    int ret = 0;
    while(true) {
        ret = getMeasurement(sens_ptr);
        if (ret != DHT20_OK) {
            printf("DHT measurement reading failed with error value %d\n", ret);
        }else {
            printf("Measurements: \n");
            printf("--- Temperature: %5.2f C°", getTemperature(sens_ptr));
            printf("--- Humidity: %5.2f \%RH\n", getHumidity(sens_ptr));
        }

        uptime++;
        sleep_ms(4000);
    }
}
int main() {
    stdio_init_all();

    sleep_ms(4000);

    printf("DHT20 test\n");

    if ( setupDHT() ) {
        printf("Exit program\n");
        while(1) {
            sleep_ms(1000);
        }
    }

    loop();
}