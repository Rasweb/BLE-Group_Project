#include <stdio.h>
#include "ESP32_Onboard_Led.h"
#include "driver/gpio.h"

#define RGB_LED_STATE 0
#define RGB_LED_PIN GPIO_NUM_8

void app_main(void)
{
    int temp_var = 0;
    onboard_rgb_led_t *rgb_led = rgb_led_init(RGB_LED_STATE, NULL, RGB_LED_PIN);
    if (rgb_led == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialize component_name");
    }

    // gpio_set_level(RGB_LED_PIN, 0);
    rgb_led_configure(rgb_led);

    rgb_led->led_state = temp_var;
    rgb_led_set_state(rgb_led);
}