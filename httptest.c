/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "stdio.h"
#include <stdlib.h>
#include "pico/cyw43_arch.h"
#include "lwip/apps/http_client.h"
#include "cJSON.h"

#define SERVER_URL ""
#define SERVER_PORT 0
#define RELATIVE_URL "/"


char myBuff[4096];

void result(void *arg, httpc_result_t httpc_result,
            u32_t rx_content_len, u32_t srv_res, err_t err)
{
    printf("\ntransfer complete\n");
    printf("local result=%d\n", httpc_result);
    printf("http result=%d\n", srv_res);
}
err_t headers(httpc_state_t *connection, void *arg,
              struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
{
    printf("\nheaders recieved\n");
    printf("content length=%d\n", content_len);
    printf("header length %d\n", hdr_len);
    pbuf_copy_partial(hdr, myBuff, hdr->tot_len, 0);
    printf("headers \n");
    printf("%s", myBuff);
    return ERR_OK;
}
char bodyData[4096];
err_t body(void *arg, struct altcp_pcb *conn,
           struct pbuf *p, err_t err)
{
    printf("body\n");
    // pbuf_copy_partial(p, myBuff, p->tot_len, 0);
    // pbuf_copy_partial(p, bodyData, p->tot_len, 0);
    //int s = strlen(myBuff);
    printf("\n------   data -------\n");
    printf("%s\n", bodyData);
    printf("---------------------\n");
    printf("body length %d\n", p->tot_len);
    //printf("%s", myBuff);
    return ERR_OK;
}

volatile uint sendTimer = 0;
volatile uint8_t request_pending = 0;

void getdata_result(void *arg, httpc_result_t httpc_result,
            u32_t rx_content_len, u32_t srv_res, err_t err)
{
    printf("\ntransfer complete\n");
    printf("local result=%d\n", httpc_result);
    printf("http result=%d\n", srv_res);
    
    // free(settingsPtr);
    // settingsPtr = NULL;
    request_pending = 0;
}
err_t getdata_headers(httpc_state_t *connection, void *arg,
              struct pbuf *hdr, u16_t hdr_len, u32_t content_len)
{
    printf("\nheaders recieved\n");
    // printf("content length=%d\n", content_len);
    // printf("header length %d\n", hdr_len);
    // pbuf_free(hdr);
    return ERR_OK;
}
#define CONN_LED_PIN 18
err_t getdata_body(void *arg, struct altcp_pcb *conn,
           struct pbuf *p, err_t err)
{
    pbuf_copy_partial(p, bodyData, p->tot_len, 0);
    printf("\n------   data -------\n");
    printf("%s\n", bodyData);

    cJSON *json = cJSON_Parse(bodyData);
    if (json != NULL) {
        cJSON *stateCJ = cJSON_GetObjectItemCaseSensitive(json, "led_state");
        // printf("tmp: %s \n",tmpJ->valuestring);
        if (cJSON_IsNumber(stateCJ)) {
            int tmpVal = stateCJ->valueint;
            printf("led_state: %d", tmpVal);

            gpio_put(CONN_LED_PIN, tmpVal <= 0 ? 0 : 1);
        }
    }
    cJSON_Delete(json);
    json = NULL;

    // uint8_t led = atoi(bodyData);
    // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led);
    // printf("led: %d\n",led);
    printf("---------------------\n");

    pbuf_free(p);
    return ERR_OK;
}

void sendGet() {
    static httpc_connection_t settings;
    // if (settingsPtr != NULL) {
    //     printf("SettingsPtr not NULL, skip send\n");
    //     return;
    // }
    // settingsPtr = (httpc_connection_t*)malloc(sizeof(httpc_connection_t));

    if (request_pending) {
        printf("Request still pending, skip send\n");
        return;
    }
    request_pending = 1;

    printf("Sending get request\n");

    settings.result_fn = getdata_result;
    settings.headers_done_fn = getdata_headers;

    ip_addr_t ip = IPADDR4_INIT_BYTES(0, 0, 0, 0);
    // const ip_addr_t ip;
    // ip.u_addr.ip4 = 22;
    err_t err = httpc_get_file(
        &ip,
        SERVER_PORT,
        "/settings-get",
        &settings,
        getdata_body,
        NULL,
        NULL);
    // err_t err = httpc_get_file_dns(
    //     SERVER_URL,
    //     SERVER_PORT,
    //     "/get-ledstate",
    //     settingsPtr,
    //     getdata_body,
    //     NULL,
    //     NULL);
    printf("status %d\n", err);
}

#ifndef WIFI_SSID
#define WIFI_SSID ""
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD ""
#endif

#ifndef TEST_DEF
#define TEST_DEF ""
#endif



int main() {
    bool r = stdio_init_all();
    sleep_ms(4000);
    if (r == false) {
        while (1) {
            sleep_ms(1000);
        }
    }
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    printf("WIFI_SSID: %s\n", WIFI_SSID);
    printf("WIFI_PASSWORD: %s\n", WIFI_PASSWORD);
    printf("TEST_DEF: %s\n", TEST_DEF);
    cyw43_arch_enable_sta_mode();
    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("failed to connect.\n");
        return 1;
    } else {
        printf("Connected.\n");
    }

    gpio_init(CONN_LED_PIN);
    gpio_set_dir(CONN_LED_PIN, GPIO_OUT);
    

    uint uptime = 0;

    sleep_ms(2000);
    
    // httpc_connection_t settings;
    // settings.result_fn = result;
    // settings.headers_done_fn = headers;
    // err_t err = httpc_get_file_dns(
    //     SERVER_URL,
    //     SERVER_PORT,
    //     RELATIVE_URL,
    //     &settings,
    //     body,
    //     NULL,
    //     NULL);

    // printf("status: %d\n", err);

    // httpc_connection_t settings;
    // settings.result_fn = getdata_result;
    // settings.headers_done_fn = getdata_headers;
    // err_t err = httpc_get_file_dns(
    //     SERVER_URL,
    //     SERVER_PORT,
    //     "/",
    //     &settings,
    //     getdata_body,
    //     NULL,
    //     NULL);
    // printf("status %d\n", err);

    
    while (true) {
        if (!request_pending && ++sendTimer >= 3) {
            sendTimer = 0;
            sendGet();
        }
        // if (sendTimer >= 1) {
        //     sendTimer = 0;
        // }
        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        // sleep_ms(500);
        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        // sleep_ms(500);
        uptime++;
        // printf("uptime: %d\n", uptime);
        sleep_ms(1000);
    }
}

// cmake .. -DPICO_BOARD=pico_w