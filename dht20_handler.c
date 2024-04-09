#include "dht20_handler.h"

static DHT20 sens;
static DHT20 *sens_ptr = &sens;

int dht20_setupDHT()
{
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
int dht20_read(float *temp_to_write, float *hum_to_write) {
    int ret = getMeasurement(sens_ptr);
    if (ret != DHT20_OK) {
        printf("DHT measurement reading failed with error value %d\n", ret);
        return -1;
    }else {
        // printf("Measurements: \n");
        *temp_to_write = getTemperature(sens_ptr);
        *hum_to_write = getHumidity(sens_ptr);
        // printf("--- Temperature: %5.2f C°", *temp_to_write);
        // printf("--- Humidity: %5.2f \%RH\n", *hum_to_write);
    }
    // sleep_ms(4000);
    return 0;
}