#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "dht20_handler.h"
#include "math.h"

#include "web_handler.h"
#include "cJSON.h"

// #include "src/calc.h"

#define TEMP_ADC_PIN 26
#define TEMP_ADC_SELECT 0
#define POTENT_ADC_PIN 28
#define POTENT_ADC_SELECT 2

#define READ_STATE_ALL 0
#define READ_STATE_DHT_ALL 1
#define READ_STATE_MHT_ONLY 2

#define STATE_LED_PIN 18

// FIX: state should stay in 0-2 range, now goes 3

void setup_potentiometer() {
    adc_init();
    adc_gpio_init(POTENT_ADC_PIN);
    // adc_select_input(2);
}
void state1() {
    float dht_temp, dht_hum;

    adc_select_input(TEMP_ADC_SELECT);
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

        webhandler_post_sensordata(dht_temp, dht_hum);
    }
}
void state2() {
    float dht_temp, dht_hum;

    if (dht20_read(&dht_temp, &dht_hum) != 0) {
        printf("DHT reading failed.\n");
    }else {
        printf("--- DHT Measurements\n");
        printf("--- Temperature: %5.2f C°", dht_temp);
        printf("--- Humidity: %5.2f \%RH\n", dht_hum);
    }
}
void state3() {
    adc_select_input(TEMP_ADC_SELECT);
    // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    const float conversion_factor = 3.3f / (1 << 12);   //8*10^-4 3.3 / 4096
    uint16_t result = adc_read(); // 0x430 = 1072
    float c_per_mv = 19.5f;
    float tmp = ((result * conversion_factor) * 1000-400) / c_per_mv;
    printf("Temp: %f'c Raw value: %d, voltage: %f V\n", tmp, result, result * conversion_factor);
}

void on_get_settings(char *data, size_t size) {
    printf("ongetdata\n");
        // ptr()
    cJSON *json = cJSON_Parse(data);
    if (json != NULL) {
        printf("is json data\n");
        cJSON *stateCJ = cJSON_GetObjectItemCaseSensitive(json, "ledState");
        // printf("tmp: %s \n",tmpJ->valuestring);
        if (cJSON_IsNumber(stateCJ)) {
            int tmpVal = stateCJ->valueint;
            printf("led_state: %d", tmpVal);

            gpio_put(STATE_LED_PIN, tmpVal == 1 ? 1 : 0);
            // gpio_put(CONN_LED_PIN, tmpVal <= 0 ? 0 : 1);
        }else {
            printf("not number\n");
        }
        cJSON_Delete(json);
    }
    json = NULL;
}
void loopFiller() {
    printf("Keep app alive for usb\n");
    while(1) {
        tight_loop_contents();
        // sleep_ms(1000);
    }
}
int main() {
    stdio_init_all();

    sleep_ms(4000);

    printf("Temperature & Humidity sensor\n");

    gpio_init(STATE_LED_PIN);
    gpio_set_dir(STATE_LED_PIN, GPIO_OUT);

    adc_init();
    adc_gpio_init(TEMP_ADC_PIN);
    
    setup_potentiometer();

    //DHT20 init failed
    if (dht20_setupDHT() != 0) {
        loopFiller();
    }

    if (webhandler_setup() != 0) {
        // printf("Wifi connection failed")
        loopFiller();
    }

    webhandler_getsettings_callback = on_get_settings;
    webhandler_getsettings();


    // gpio_init(PICO_DEFAULT_LED_PIN);
    // gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);
    unsigned int uptime = 0;

    uint8_t state = READ_STATE_ALL;

    
    while(1) {
        sleep_ms(1000);
        continue;

        switch (state) {
            case READ_STATE_ALL:
                state1();
                break;
            case READ_STATE_DHT_ALL:
                state2();
                break;
            case READ_STATE_MHT_ONLY:
                state3();
                break;
            default:
                break;
        }   

        adc_select_input(POTENT_ADC_SELECT);
        uint8_t result = adc_read() * 100 / (1 << 12);
        printf("\nPotentti: %d\n",result);

        state = roundf(((float)result) / 30.0f);
        printf("State: %d\n", state);
        // printf("Uptime: %d\n", uptime);
        uptime++;

        webhandler_getsettings();
        sleep_ms(4000);
    }
    return 0;
}