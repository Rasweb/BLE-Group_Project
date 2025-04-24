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

#define GATTC_TAG "CLIENT"
#define FROM_S3 "From S3"
#define REMOTE_SERVICE_UUID 0x00FF
#define REMOTE_NOTIFY_CHAR_UUID 0xFF01
#define PROFILE_NUM 1
#define PROFILE_A_APP_ID 0
#define INVALID_HANDLE 0

static char remote_device_name[ESP_BLE_ADV_NAME_LEN_MAX] = "S3-SERVER";
static bool connect = false;
static bool get_server = false;
static esp_gattc_char_elem_t *char_elem_result = NULL;
static esp_gattc_descr_elem_t *descr_elem_result = NULL;

extern uint16_t client_conn;
extern esp_gatt_if_t client_interface;

struct gattc_profile_inst
{
    esp_gattc_cb_t gattc_cb;
    uint16_t gattc_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_start_handle;
    uint16_t service_end_handle;
    uint16_t char_handle;
    esp_bd_addr_t remote_bda;
};

extern struct gattc_profile_inst gl_profile_tab[];

void ble_run();

void ble_send();

uint8_t get_received_data();

void clear_received_data();