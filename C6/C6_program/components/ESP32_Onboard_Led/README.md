# RGB Onboard Led instructions
Instructions for how to use the RGB onboard led on the ESP32-C6.

## Include the header file
```c
#include "ESP32_Onboard_Led.h"
```

## Create these macros:
```c
#define RGB_LED_STATE 0
#define RGB_LED_PIN GPIO_NUM_8
```

## Initializing the component:
``` c
onboard_rgb_led_t *component_name = rgb_led_init(RGB_LED_STATE, NULL, RGB_LED_PIN);
    if (component_name == NULL)
    {
        ESP_LOGE(TAG, "Failed to initialize component_name");
    }
	
gpio_set_level(RGB_LED_PIN, 0);
rgb_led_configure(component_name);
```

## To control the led
``` c
component_name->led_state = 1;  
rgb_led_set_state(component_name);
rgb_led_free(component_name);
```