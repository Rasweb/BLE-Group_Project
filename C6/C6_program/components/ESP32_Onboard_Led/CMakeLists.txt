idf_component_register(SRCS "esp32_onboard_led.c"
                    INCLUDE_DIRS "include"
                    REQUIRES driver led_strip)