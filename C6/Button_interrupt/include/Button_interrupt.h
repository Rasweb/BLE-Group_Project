#pragma once
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"


typedef enum {
    PULLDOWN,
    PULLUP,
    NEITHER
}strapping_mode;

typedef enum {
    BTN_IDLE = 0,
    BTN_PRESSED,
    BTN_RELEASED,

}btn_i_state;


typedef struct{
    gpio_num_t pin;
    volatile bool interrupt_triggered;
    bool flag;
    btn_i_state STATE;
    TickType_t last_trig_time;
    TickType_t debounce_time;
    TickType_t debounce_start_time;
    int counter; //debugging
    strapping_mode mode;
    TaskHandle_t task_handle;
}btn_i_conf;

typedef btn_i_conf* btn_i_handle;

btn_i_handle btn_i_init(const gpio_num_t pin, const strapping_mode mode, const int debounce_ms);


void btn_i_update(btn_i_handle button);

bool btn_pressed(const btn_i_handle button);

void btn_clear_flag(btn_i_handle button);