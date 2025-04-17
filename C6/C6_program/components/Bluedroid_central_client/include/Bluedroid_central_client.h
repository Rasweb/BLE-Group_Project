#pragma once
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "nvs.h"
#include "nvs_flash.h"

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#define GATTC_TAG "GATTC_DEMO"
#define REMOTE_SERVICE_UUID        0x00FF
#define REMOTE_NOTIFY_CHAR_UUID    0xFF01
#define PROFILE_NUM      1
#define PROFILE_A_APP_ID 0
#define INVALID_HANDLE   0


static char remote_device_name[ESP_BLE_ADV_NAME_LEN_MAX] = "S3-SERVER";
static bool connect    = false;
static bool get_server = false;
static esp_gattc_char_elem_t *char_elem_result   = NULL;
static esp_gattc_descr_elem_t *descr_elem_result = NULL;

extern esp_gatt_if_t client_interface;
extern uint16_t client_conn;

// esp_gatt_if_t client_if;

typedef struct gattc_profile_inst{
    esp_gattc_cb_t gattc_cb;
    uint16_t gattc_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_start_handle;
    uint16_t service_end_handle;
    uint16_t char_handle;
    esp_bd_addr_t remote_bda;

    // uint16_t client_conn;
    // esp_gatt_if_t client_interface;
    // struct gattc_profile_inst gl_profile_tab[];
    // gattc_profile_inst gl_profile_tab[];
    
};
typedef struct gattc_profile_inst * ble_handle;

// uint16_t client_conn;
// extern struct esp_gatt_if_t client_interface1;
extern struct gattc_profile_inst gl_profile_tab[];

void gattc_profile_event_handler(ble_handle ble, esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

ble_handle run_ble_setup();

void ble_run_2();

void ble_write(ble_handle ble);

void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
void esp_gattc_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);




void func(void);
