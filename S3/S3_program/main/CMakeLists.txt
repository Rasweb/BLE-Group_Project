idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES 
                    Bluedroid_peripheral_server 
                    Battery_monitor 
                    Button_interrupt 
                    ULN2003_stepper_motor_driver )