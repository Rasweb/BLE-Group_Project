#include <stdio.h>
#include "Bluedroid_peripheral_server.h"
#include "Battery_monitor.h"
#include "Button_interrupt.h"
#include "ULN2003_stepper_motor_driver.h"

#define BATTERY_INTERVAL (10*1000)

#define BATTERY_PIN GPIO_NUM_8 // ADC1 CHANNEL 7
#define STEPPER_IN1 GPIO_NUM_4
#define STEPPER_IN2 GPIO_NUM_5
#define STEPPER_IN3 GPIO_NUM_6
#define STEPPER_IN4 GPIO_NUM_7
#define BTN_CW      GPIO_NUM_15
#define BTN_CCW     GPIO_NUM_16

void app_main(void)
{

    batt_handle battery = battery_init(BATTERY_PIN);
    stepper_motor_handle stepper_motor = stepper_motor_init(STEPPER_IN1, STEPPER_IN2, STEPPER_IN3, STEPPER_IN4);
    btn_i_handle btn_cw = btn_i_init(BTN_CW, PULLDOWN, 100);
    btn_i_handle btn_ccw = btn_i_init(BTN_CCW, PULLDOWN, 100);

    ble_server_run();

    TickType_t last_tick = 0;
    while (1)
    {
        battery_update(battery);
        if(btn_pressed(btn_cw))
        {
            clear_recieved_data(); //overridea så att det inte blir kö av upp/ned
            stepper_motor_move(stepper_motor, STPR_MTR_CW, 3600); //för intrimning/demo UPP
            btn_clear_flag(btn_cw);
        }
        else if(btn_pressed(btn_ccw))
        {
            clear_recieved_data(); //overridea så att det inte blir kö av upp/ned
            stepper_motor_move(stepper_motor, STPR_MTR_CCW, 3600); //för intrimning/demo NED
            btn_clear_flag(btn_ccw);
        }
        btn_i_update(btn_cw);
        btn_i_update(btn_ccw);
        stepper_motor_update(stepper_motor);


        TickType_t current_tick = xTaskGetTickCount();
        if ((current_tick - last_tick) >= pdMS_TO_TICKS(BATTERY_INTERVAL))
        {
            batt_charge charge = battery_get_charge(battery);
            if(charge == BATT_CRHG_UNKNOWN ||
               charge == BATT_CHRG_0 ||
               charge == BATT_CHRG_10)
            {
                printf("Battery low, sending notification\n");
                send_notification(true);
                if(get_stepper_motor_position(stepper_motor)!= STPR_MTR_OPEN)
                {
                    stepper_motor_move(stepper_motor, STPR_MTR_CW, 1024);// UPP
                }
            }
            else
            {
                printf("Send false\n");
                send_notification(false);
            }

            last_tick = current_tick;
        }
        if (get_recieved_data()                      == 0x06 &&
            stepper_motor_ready(stepper_motor)       == true &&
            get_stepper_motor_position(stepper_motor)!= STPR_MTR_OPEN) //kör öppning endast om den inte redan körs eller inte redan är öppnad
        {
            printf("Opening lock\n");
            stepper_motor_move(stepper_motor, STPR_MTR_CW, 3600);
            clear_recieved_data();
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}