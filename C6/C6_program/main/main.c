#include <stdio.h>
#include "Bluedroid_central_client.h"
// esp_gatt_if_t client_interface;
// uint16_t client_conn;
ble_handle ble;
void app_main(void)
{
ble = run_ble_setup();
    while (1)
    {
        ble_write(ble);

        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("hej\n");
    }

}