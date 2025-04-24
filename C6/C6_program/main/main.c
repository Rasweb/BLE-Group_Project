#include <stdio.h>
#include "Button_interrupt.h"
#include "Bluedroid_central_client.h"
#include "freertos/FreeRTOS.h"
#include "esp32_onboard_led.h"
#include "driver/gpio.h"

#define RGB_LED_STATE 0
#define RGB_LED_PIN GPIO_NUM_8

#define BTN_1 GPIO_NUM_2
#define BTN_2 GPIO_NUM_3
#define BTN_3 GPIO_NUM_4

typedef enum
{
    ACCESS,
    DENIED,
    WAITING
} logic_e;

btn_i_handle btn1;
btn_i_handle btn2;
btn_i_handle btn3;

int correct_sequence[] =
    {
        1, 2, 3, 3, 2, 1};

logic_e button_logic();

void app_main(void)
{
    onboard_rgb_led_t *rgb_led = rgb_led_init(RGB_LED_STATE, NULL, RGB_LED_PIN);
    if (rgb_led == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialize component_name");
    }

    gpio_set_level(RGB_LED_PIN, 0); // Kanske inte behövs
    rgb_led_configure(rgb_led);

    logic_e state = WAITING;
    btn1 = btn_i_init(BTN_1, PULLUP, 250);
    btn2 = btn_i_init(BTN_2, PULLUP, 250);
    btn3 = btn_i_init(BTN_3, PULLUP, 250);

    ble_run();
    TickType_t last_tick = 0;

    while (state != DENIED) // ändra till waiting senare
    {
        TickType_t current_tick = xTaskGetTickCount();
        btn_i_update(btn1);
        btn_i_update(btn2);
        btn_i_update(btn3);

        state = button_logic();
        vTaskDelay(pdMS_TO_TICKS(10));
        if (state == ACCESS)
        {
            printf("HOORAAAY!\n");
            ble_send();
        }
        if (get_received_data() == 0x06)
        {
            printf("Battery low!\n");
            // clear_received_data();
            rgb_led->led_state = 1;
            rgb_led_set_state(rgb_led);
            last_tick = current_tick;
        }
        else if (get_received_data() == 0x00)
        {
            rgb_led->led_state = 2;
            rgb_led_set_state(rgb_led);
            last_tick = current_tick;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
logic_e button_logic()
{
    static int attempt = 0;
    static int currentIndex = 0;
    static int user_sequence[6];
    static int lastindex = 0;

    if (attempt < 3)
    {
        if (currentIndex < 6)
        {

            if (btn_pressed(btn1))
            {
                printf("btn 1\n");
                user_sequence[currentIndex] = 1;
                currentIndex++;
                btn_clear_flag(btn1);
            }
            if (btn_pressed(btn2))
            {
                printf("btn 2\n");
                user_sequence[currentIndex] = 2;
                currentIndex++;
                btn_clear_flag(btn2);
            }
            if (btn_pressed(btn3))
            {
                printf("btn 3\n");
                user_sequence[currentIndex] = 3;
                currentIndex++;
                btn_clear_flag(btn3);
            }
            if (currentIndex != lastindex)
            {
                lastindex++;

                if (user_sequence[currentIndex - 1] == correct_sequence[currentIndex - 1])
                {
                    if (currentIndex == 6)
                    {
                        printf("Access granted!\n\n");
                        attempt = 0;
                        currentIndex = 0; // ta bort sen
                        lastindex = 0;    // ta bort sen
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
    if (attempt == 3)
    {
        printf("Access Denied!\n");
        return DENIED;
    }
    return WAITING;
}
// if (i >= 500)
// {
//     i = 0;
//     if (get_received_data() == 0x01)
//     {
//         printf("Good [%.2x]\n", get_received_data());
//         clear_received_data();
//     }
//     else
//     {
//         printf("Bad [%.2x]\n", get_received_data());
//     }
//     ble_send();
// }
// vTaskDelay(pdMS_TO_TICKS(10));
