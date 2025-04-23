#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"


typedef enum{
    STPR_MTR_NONE = -1,
    STPR_MTR_CCW,
    STPR_MTR_CW,
}STPR_MTR_DIRECTION;

typedef struct{
    gpio_num_t in1;
    gpio_num_t in2;
    gpio_num_t in3;
    gpio_num_t in4;
    STPR_MTR_DIRECTION direction;
    int16_t current_phase;
    int16_t target_steps;
    int16_t steps_taken;
    int64_t last_step_time;

}stepper_motor_t;

typedef stepper_motor_t* stepper_motor_handle;



stepper_motor_handle stepper_motor_init(const gpio_num_t in1, const gpio_num_t in2, const gpio_num_t in3, const gpio_num_t in4);

void stepper_motor_move(stepper_motor_handle stepper, const STPR_MTR_DIRECTION direction, const int16_t degrees);

void stepper_motor_update(stepper_motor_handle stepper);
