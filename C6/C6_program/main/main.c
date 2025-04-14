#include <stdio.h>
#include "Button_interrupt.h"
#include "freertos/FreeRTOS.h"

btn_i_handle btn1;
btn_i_handle btn2;
btn_i_handle btn3;
int correctsequence[] = {
    1, 2, 3, 3, 2, 1};

void app_main(void)
{
    btn1 = btn_i_init(4, PULLUP, 100);
    btn2 = btn_i_init(21, PULLUP, 100);
    btn3 = btn_i_init(13, PULLUP, 100);
    int attempt = 0;
    while (attempt < 3)
    {

        int currentIndex = 0;
        int user_sequence[6];
        int lastindex = 0;

        while (currentIndex < 6)
        {
            btn_i_update(btn1);
            btn_i_update(btn2);
            btn_i_update(btn3);
            // lägg in vtaskDelay
            vTaskDelay(pdMS_TO_TICKS(10));

            if (btn_pressed(btn1))
            {
                // printf("btn 1\n");
                user_sequence[currentIndex] = 1;
                currentIndex++;
                btn_clear_flag(btn1);
            }
            if (btn_pressed(btn2))
            {
                user_sequence[currentIndex] = 2;
                currentIndex++;
                btn_clear_flag(btn2);
            }
            if (btn_pressed(btn3))
            {
                user_sequence[currentIndex] = 3;
                currentIndex++;
                btn_clear_flag(btn3);
            }
            if (currentIndex != lastindex)
            {
                lastindex++;

                if (user_sequence[currentIndex - 1] == correctsequence[currentIndex - 1])
                {
                    printf("Correct\n");
                }
                else
                {
                    printf("Wrong!\n");
                    currentIndex = 0;
                    lastindex = 0;
                }
            }
        }
    }
}