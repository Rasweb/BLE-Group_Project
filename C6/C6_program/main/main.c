#include <stdio.h>
#include "Bluedroid_central_client.h"

void app_main(void)
{
ble_run();
    while (1)
    {
        ble_send();
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("hej\n");
    }

}