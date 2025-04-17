#include <stdio.h>
#include "Bluedroid_central_client.h"
bt_handle bt;
void app_main(void)
{
    // Initialize NVS.
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    // #if CONFIG_EXAMPLE_CI_PIPELINE_ID
    // memcpy(remote_device_name, esp_bluedroid_get_example_name(), sizeof(remote_device_name));
    // #endif
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret)
    {
        ESP_LOGE(GATTC_TAG, "%s initialize controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret)
    {
        ESP_LOGE(GATTC_TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret)
    {
        ESP_LOGE(GATTC_TAG, "%s init bluetooth failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret)
    {
        ESP_LOGE(GATTC_TAG, "%s enable bluetooth failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    // register the  callback function to the gap module
    ret = esp_ble_gap_register_callback(esp_gap_cb);
    if (ret)
    {
        ESP_LOGE(GATTC_TAG, "%s gap register failed, error code = %x", __func__, ret);
        return;
    }

    // register the callback function to the gattc module
    ret = esp_ble_gattc_register_callback(esp_gattc_cb);
    if (ret)
    {
        ESP_LOGE(GATTC_TAG, "%s gattc register failed, error code = %x", __func__, ret);
        return;
    }

    ret = esp_ble_gattc_app_register(PROFILE_A_APP_ID);
    if (ret)
    {
        ESP_LOGE(GATTC_TAG, "%s gattc app register failed, error code = %x", __func__, ret);
    }
    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
    if (local_mtu_ret)
    {
        ESP_LOGE(GATTC_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
    }
    TickType_t previousEvent = 0;
    uint8_t data_to_send[] = {0x01, 0x02};  // exempeldata
    bool hej = true;
    while(1){
        TickType_t currentick = xTaskGetTickCount();
        if(currentick - previousEvent >= pdMS_TO_TICKS(7000))
        {
            
            // esp_ble_gattc_write_char(
            // bt.gattc_if,
            // bt.conn_id,
            // bt.char_handle,
            // sizeof(data_to_send),
            // data_to_send,
            // ESP_GATT_WRITE_TYPE_NO_RSP,       // eller ESP_GATT_WRITE_TYPE_NO_RSP
            // ESP_GATT_AUTH_REQ_NONE
            // );
            previousEvent = currentick;
            printf("bajs\n");
        }
        printf(".\n");
        vTaskDelay(pdMS_TO_TICKS(1000));


    }
}