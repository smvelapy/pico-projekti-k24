#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "dht20_handler.h"
#include "math.h"

#include "web_handler.h"
#include "cJSON.h"

#include "lcd_handler.h"


// ###################
// #####--PINS--######
// ###################
#define TEMP_ADC_PIN 26
#define TEMP_ADC_SELECT 0
#define POTENT_ADC_PIN 28
#define POTENT_ADC_SELECT 2
#define STATE_LED_PIN 18


// #######################
// #####--SETTINGS--######
// #######################
#define SENSOR_TYPE_NONE 0
#define SENSOR_TYPE_DHT 1
#define SENSOR_TYPE_MHT 2

#define OPERATING_MODE_NONE 0
#define OPERATING_MODE_DHT 1
#define OPERATING_MODE_MHT 2

#define UPDATE_INTERVAL_IN_SECONDS 2
#define POST_INTERVAL_IN_SECONDS 5

float temp = .0f;
float hum = .0f;
uint8_t operating_mode = OPERATING_MODE_DHT;
uint8_t sensor_readed = SENSOR_TYPE_NONE;

void setup_potentiometer() {
    adc_init();
    adc_gpio_init(POTENT_ADC_PIN);
    // adc_select_input(2);
}
float read_mcp_temp() {
    adc_select_input(TEMP_ADC_SELECT);
    // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    const float conversion_factor = 3.3f / (1 << 12);   //8*10^-4 3.3 / 4096
    uint16_t result = adc_read(); // 0x430 = 1072
    float c_per_mv = 19.5f;
    float tmp = ((result * conversion_factor) * 1000-400) / c_per_mv;
    printf("Temp: %f'c Raw value: %d, voltage: %f V\n", tmp, result, result * conversion_factor);
    return tmp;
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
char lcd_msg_buffer[20];
void updateLCD() {
    // sprintf(lcd_msg_buffer, "Temp: %5.2f C°", temp);
    // lcdhandler_write_line(lcd_msg_buffer, 0);
    // sprintf(lcd_msg_buffer, "HUM: %5.2f \%RH", hum);
    // lcdhandler_write_line(lcd_msg_buffer, 1);
}
void loopFiller() {
    printf("Keep app alive for usb\n");
    while(1) {
        tight_loop_contents();
        // sleep_ms(1000);
    }
}
bool get_settings_callback(struct repeating_timer *t) {
    printf("Get settings callback\n");
    webhandler_getsettings(operating_mode);
    return true;
}
bool post_data_callback(struct repeating_timer *t) {
    printf("Post data callback\n");
    webhandler_post_sensordata(sensor_readed, temp, hum);
    return true;
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

    // lcdhandler_init();

    if (webhandler_setup() != 0) {
        // printf("Wifi connection failed")
        loopFiller();
    }

    webhandler_getsettings_callback = on_get_settings;
    webhandler_getsettings(operating_mode);


    struct repeating_timer get_settings_timer;
    add_repeating_timer_ms(UPDATE_INTERVAL_IN_SECONDS * 1000, get_settings_callback, NULL, &get_settings_timer);

    struct repeating_timer post_data_timer;
    add_repeating_timer_ms(POST_INTERVAL_IN_SECONDS * 1000, post_data_callback, NULL, &post_data_timer);
    

    
    while(1) {
        // sleep_ms(1000);
        // temp = 24.0f;
        // hum = 50.0f;
        // updateLCD();
        // continue;
        adc_select_input(POTENT_ADC_SELECT);
        uint8_t result = adc_read() * 100 / (1 << 12);
        printf("\nPotentti: %d\n",result);
        // operating_mode = roundf(((float)result) / 30.0f);
        // Round number to down
        operating_mode = (uint8_t)(result / 33);
        if (operating_mode > 2) operating_mode = 2;
        printf("State: %d\n", operating_mode);

        

        switch (operating_mode) {
            case OPERATING_MODE_DHT:
                if (dht20_read(&temp, &hum) != 0) {
                    sensor_readed = SENSOR_TYPE_MHT;
                    printf("DHT reading failed.\n");
                    printf("Fallback to mcp sensor\n");
                    temp = read_mcp_temp();
                }else {
                    sensor_readed = SENSOR_TYPE_DHT;
                    printf("--- DHT Measurements\n");
                    printf("--- Temperature: %5.2f C°", temp);
                    printf("--- Humidity: %5.2f \%RH\n", hum);
                }
                // webhandler_post_sensordata(sensor_readed, temp, hum);
                break;
            case OPERATING_MODE_MHT:
                sensor_readed = SENSOR_TYPE_MHT;
                temp = read_mcp_temp();
                // webhandler_post_sensordata(sensor_readed, temp, hum);
                break;
            default:
                sensor_readed = SENSOR_TYPE_NONE;
                break;
        }

        sleep_ms(1000);   

        // printf("Uptime: %d\n", uptime);
        // uptime++;

        // webhandler_getsettings(operating_mode);
        // updateLCD();
        
    }
    return 0;
}