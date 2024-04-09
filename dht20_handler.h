#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "dht20-pico/DHT20.h"

#define DHT_I2C_INST i2c0
#define DHT_I2C_SDA 12
#define DHT_I2C_SCL 13

int dht20_setupDHT();
int dht20_read(float *temp, float *hum);
