#include <stdio.h>
#include "Bluedroid_peripheral_server.h"
#include "Battery_monitor.h"

#define BATTERY_PIN GPIO_NUM_8 // ADC1 CHANNEL 7

void app_main(void)
{
    batt_handle battery = battery_init(BATTERY_PIN);
    ble_server_run();

    TickType_t last_tick = 0;
    while (1)
    {
        battery_update(battery);

        TickType_t current_tick = xTaskGetTickCount();
        if ((current_tick - last_tick) >= pdMS_TO_TICKS(5000))
        {
            batt_charge charge = battery_get_charge(battery);
            if(charge == BATT_CRHG_UNKNOWN ||
               charge == BATT_CHRG_0 ||
               charge == BATT_CHRG_10)
            {
                send_notification(true);
            }
            else
            {
                send_notification(false);
            }

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