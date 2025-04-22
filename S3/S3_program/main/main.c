#include <stdio.h>
#include "Bluedroid_peripheral_server.h"

void app_main(void)
{
    ble_server_run();

    TickType_t last_tick = 0; // Save current tick count
    while (1)
    {
        TickType_t current_tick = xTaskGetTickCount();
        if ((current_tick - last_tick) >= pdMS_TO_TICKS(5000))
        {
            send_notification(true);
            last_tick = current_tick;
        }
        if (get_recieved_data() == 0x01)
        {
            printf("Opening lock... [%d]\n", get_recieved_data());
            // kod för motoröppning här
            clear_recieved_data();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}