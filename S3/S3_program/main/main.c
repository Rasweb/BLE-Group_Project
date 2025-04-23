#include <stdio.h>
#include "Bluedroid_peripheral_server.h"
#include "Battery_monitor.h"

#define BATTERY_INTERVAL (5*1000)
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
        if ((current_tick - last_tick) >= pdMS_TO_TICKS(BATTERY_INTERVAL))
        {
            batt_charge charge = battery_get_charge(battery);
            // printf("charge enum =%d\n", charge);
            if(charge == BATT_CRHG_UNKNOWN ||
               charge == BATT_CHRG_0 ||
               charge == BATT_CHRG_10)
            {
                printf("Send true\n");
                send_notification(true);
                // KÖR MOTOR KOD HÄR FÖR ATT LÅSA UPP så att inte enheten blir instängd
            }
            else
            {
                printf("Send false\n");
                send_notification(false);
            }

            last_tick = current_tick;
        }

        if (get_recieved_data() == 0x01)
        {
            printf("Opening lock\n");
            // kod för motoröppning här
            clear_recieved_data();
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}