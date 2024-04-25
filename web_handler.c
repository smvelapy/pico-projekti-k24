#include "web_handler.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include <stdlib.h>
#include "pico/cyw43_arch.h"
#include "lwip/apps/http_client.h"
#include "cJSON.h"

#define WIFI_SSID "backup-jako"
#define WIFI_PASSWORD "99932282"
#define SERVER_URL "192.168.1.102"
#define SERVER_PORT 8000

void (*webhandler_getsettings_callback) (char *buffer, size_t size);
int webhandler_setup()
{
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    printf("WIFI_SSID: %s\n", WIFI_SSID);
    printf("WIFI_PASSWORD: %s\n", WIFI_PASSWORD);
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
        return 0;
    }
}
void getsettings_result(void *arg, httpc_result_t httpc_result,
            u32_t rx_content_len, u32_t srv_res, err_t err)
{
    printf("\ntransfer complete\n");
    printf("local result=%d\n", httpc_result);
    printf("http result=%d\n", srv_res);
    
    // free(settingsPtr);
    // settingsPtr = NULL;
}
err_t getsettings_headers(httpc_state_t *connection, void *arg,
              struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
{
    printf("\nheaders recieved\n");
    // printf("content length=%d\n", content_len);
    // printf("header length %d\n", hdr_len);
    // pbuf_free(hdr);
    return ERR_OK;
}
#define CONN_LED_PIN 18

#define BODY_SIZE 128
char bodyData[BODY_SIZE];
err_t getsettings_body(void *arg, struct altcp_pcb *conn,
           struct pbuf *p, err_t err)
{
    if (p->tot_len > BODY_SIZE) {
        printf("WARNING the size exceeds the buffer's size!\n");       
    }else {
        pbuf_copy_partial(p, bodyData, p->tot_len, 0);
        printf("\n------   data -------\n");
        printf("%s\n", bodyData);

        if (webhandler_getsettings_callback != NULL) {
            webhandler_getsettings_callback(bodyData, BODY_SIZE);
        }
        // ptr()
        // cJSON *json = cJSON_Parse(bodyData);
        // if (json != NULL) {
        //     cJSON *stateCJ = cJSON_GetObjectItemCaseSensitive(json, "led_state");
        //     // printf("tmp: %s \n",tmpJ->valuestring);
        //     if (cJSON_IsNumber(stateCJ)) {
        //         int tmpVal = stateCJ->valueint;
        //         printf("led_state: %d", tmpVal);

        //         gpio_put(CONN_LED_PIN, tmpVal <= 0 ? 0 : 1);
        //     }
        // }
        // cJSON_Delete(json);
        // json = NULL;
    }

    pbuf_free(p);
    return ERR_OK;
}

void webhandler_getsettings(unsigned char sensor_readed) {
    static httpc_connection_t settings;
    // if (settingsPtr != NULL) {
    //     printf("SettingsPtr not NULL, skip send\n");
    //     return;
    // }
    // settingsPtr = (httpc_connection_t*)malloc(sizeof(httpc_connection_t));

    settings.result_fn = getsettings_result;
    settings.headers_done_fn = getsettings_headers;

    // ip_addr_t ip = IPADDR4_INIT_BYTES(192, 168, 1, 102);
    // ip_addr_t ip;
    // IP4_ADDR(&ip, 192, 168, 253, 45);   
    // IP4_ADDR(&ip, 192, 168, 253, 45); 
    // const ip_addr_t ip;
    // ip.u_addr.ip4 = 22;
    char endpoint_buff[33];
    sprintf(endpoint_buff, "/settings-get?sensor-mode=%d", sensor_readed);
    err_t err = httpc_get_file_dns(
        SERVER_URL,
        SERVER_PORT,
        endpoint_buff,
        &settings,
        getsettings_body,
        NULL,
        NULL);
    printf("statur %d\n",err);
}

void post_sensordata_result(void *arg, httpc_result_t httpc_result,
            u32_t rx_content_len, u32_t srv_res, err_t err)
{
    printf("\ntransfer complete\n");
    printf("local result=%d\n", httpc_result);
    printf("http result=%d\n", srv_res);
}
err_t post_sensordata_headers(httpc_state_t *connection, void *arg,
              struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
{
    printf("\nheaders recieved\n");
    // printf("content length=%d\n", content_len);
    // printf("header length %d\n", hdr_len);
    // pbuf_free(hdr);
    return ERR_OK;
}
err_t post_sensordata_body(void *arg, struct altcp_pcb *conn,
           struct pbuf *p, err_t err)
{
    printf("body received: \n");
    pbuf_free(p);
    return ERR_OK;
}

void webhandler_post_sensordata(unsigned char sensor_type, float temp, float hum)
{
    printf("[post_sensordata]\n");
    static httpc_connection_t settings;
    printf("settins size: %lu\n", sizeof(settings));
    // if (settingsPtr != NULL) {
    //     printf("SettingsPtr not NULL, skip send\n");
    //     return;
    // }
    // settingsPtr = (httpc_connection_t*)malloc(sizeof(httpc_connection_t));

    settings.result_fn = post_sensordata_result;
    settings.headers_done_fn = post_sensordata_headers;

    // cJSON *jsonObj =  cJSON_CreateObject();
    // cJSON_AddNumberToObject(jsonObj, "temp", temp);
    // cJSON_AddNumberToObject(jsonObj, "hum", hum);
    // cJSON_Delete(jsonObj);

    char endpoint_buff[60];
    // sprintf(endpoint_buff, "/data-post?data={\"temp\":%.2f,\"hum\":%.2f}", temp, hum);

    sprintf(endpoint_buff, "/data-post?sensor_type=%d&temp=%.2f&hum=%.2f", sensor_type, temp, hum);
    printf("buff: %s\n", endpoint_buff);
    // ip_addr_t ip = IPADDR4_INIT_BYTES(192, 168, 1, 102);
    // ip_addr_t ip;
    // IP4_ADDR(&ip, 192, 168, 253, 45);   
    // IP4_ADDR(&ip, 192, 168, 253, 45); 
    // const ip_addr_t ip;
    // ip.u_addr.ip4 = 22;
    err_t err = httpc_get_file_dns(
        SERVER_URL,
        SERVER_PORT,
        endpoint_buff,
        &settings,
        post_sensordata_body,
        NULL,
        NULL);
 
}

// void webhandler