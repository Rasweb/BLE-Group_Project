#include <stdio.h>
#include "Button_interrupt.h"
#include "Bluedroid_central_client.h"
#include "freertos/FreeRTOS.h"

typedef enum{
    ACCESS,
    DENIED,
    WAITING
}logic_e;

btn_i_handle btn1;
btn_i_handle btn2;
btn_i_handle btn3;
int correctsequence[] = 
{
    1, 2, 3, 3, 2, 1
};
logic_e button_logic();

void app_main(void)
{
    ble_run();
    int i = 0;
  
    logic_e state = WAITING;
    btn1 = btn_i_init(4, PULLUP, 250);
    btn2 = btn_i_init(21, PULLUP, 250);
    btn3 = btn_i_init(13, PULLUP, 250);
    while(state != DENIED) //ändra till waiting senare
    {
        state = button_logic();
        vTaskDelay(pdMS_TO_TICKS(10));
        if(state == ACCESS){
            printf("HOORAAAY!\n");
        }
    }
}
logic_e button_logic()
{
    static int attempt = 0;
    static int currentIndex = 0;
    static int user_sequence[6];
    static int lastindex = 0;
      while (1)
    {
         if (attempt < 3)
    {
        if (currentIndex < 6)
        {
            btn_i_update(btn1);
            btn_i_update(btn2);
            btn_i_update(btn3);
            // lägg in vtaskDelay
            

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
                    if (currentIndex == 6)
                    {
                        printf("Access granted!\n\n");
                        attempt = 0;
                        currentIndex = 0; //ta bort sen
                        lastindex = 0; //ta bort sen
                        return ACCESS;
                    }
                    else
                    {
                        printf("Correct\n");
                        // return WAITING;
                    }
                }
                else
                {
                    printf("Wrong!\n");
                    attempt++;
                    currentIndex = 0;
                    lastindex = 0;
                    // return WAITING;
                }
            }
        }
    }
    if(attempt == 3){
        printf("Access Denied!\n");
        return DENIED;
    }
    return WAITING;
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