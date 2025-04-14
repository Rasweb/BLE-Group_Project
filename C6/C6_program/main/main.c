#include <stdio.h>
#include "Button_interrupt.h"

btn_i_handle btn1;
btn_i_handle btn2;
btn_i_handle btn3;
int correctsequence[]= {
    1,2,3,3,2,1
}

void app_main(void)
{
    btn1 = btn_i_init(4, PULLUP, 50);
    btn2 = btn_i_init(21, PULLUP, 50);
    btn3 = btn_i_init(13, PULLUP, 50);
    int attempt = 0;
    while (attempt < 3){

        int currentIndex = 0;
        int user_sequence[6];
        
        while (currentIndex < 6)
        {
            btn_i_update(btn1);
            btn_i_update(btn2);
            btn_i_update(btn3);
            
            if (btn_pressed(btn1))
            {
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
            for (int i = 0; i < 6; i++)
            {
                if (user_sequence[i] == correctsequence[i]){
                    printf("+");
                }
                else{
                    printf("Wrong code!\n");
                    i= 6;
                }
            }
        }
    }
        
}