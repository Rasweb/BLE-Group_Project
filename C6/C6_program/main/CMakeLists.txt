idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES Bluedroid_central_client Button_interrupt esp32_onboard_led)

