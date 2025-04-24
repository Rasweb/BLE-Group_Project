#include <stdio.h>
#include "Bluedroid_central_client.h"

void app_main(void)
{
    ble_run();
    int i = 0;
    while (1)
    {
        i++;
        if (i >= 500)
        {
            i = 0;
            if (get_received_data() == 0x01)
            {
                printf("Good [%.2x]\n", get_received_data());
                clear_received_data();
            }
            else
            {
                printf("Bad [%.2x]\n", get_received_data());
            }
            ble_send();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}