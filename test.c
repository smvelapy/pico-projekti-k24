#include <stdio.h>
#include "pico/stdlib.h"
#include "web_handler.h"
#include "cJSON.h"

void ongetdata(char *data, int len) {
    printf("ongetdata\n");
        // ptr()
    cJSON *json = cJSON_Parse(data);
    if (json != NULL) {
        cJSON *stateCJ = cJSON_GetObjectItemCaseSensitive(json, "led_state");
        // printf("tmp: %s \n",tmpJ->valuestring);
        if (cJSON_IsNumber(stateCJ)) {
            int tmpVal = stateCJ->valueint;
            printf("led_state: %d", tmpVal);

            // gpio_put(CONN_LED_PIN, tmpVal <= 0 ? 0 : 1);
        }
        cJSON_Delete(json);
    }
    json = NULL;
    
    // if (p->tot_len > 2) { 
    
    // }
    // pbuf_copy_partial(p, bodyData, p->tot_len, 0);
}
int main() {
    stdio_init_all();

    sleep_ms(4000);
    printf("Webhandler test\n");

    if (webhandler_setup() != 0) {
        while(1) {
            tight_loop_contents();
            sleep_ms(1000);
        }
    }
    webhandler_getsettings_callback = ongetdata;

    printf("Send getsettings\n");
    webhandler_getsettings();
    sleep_ms(3000);

    float temp = 22.0f;
    float hum = 50.0f;
    while(1) {
        printf("Send sensordata\n");
        webhandler_post_sensordata(temp++, hum++);
        // tight_loop_contents();
        sleep_ms(2000);
    }

}