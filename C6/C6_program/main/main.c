#include <stdio.h>
#include "Bluedroid_central_client.h"

void app_main(void)
{
ble_run();
int i = 0;
    while (1)
    {
        i++;
        if(i >= 1000){
            i = 0;
            ble_send();
            printf("hej\n");
        }
        // ble_read();
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}