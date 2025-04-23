#include "ULN2003_stepper_motor_driver.h"

static const char* TAG = "STEPPER_MOTOR";

static const int full_step_wave[4][4] =
{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

stepper_motor_handle stepper_motor_init(const gpio_num_t in1, const gpio_num_t in2, const gpio_num_t in3, const gpio_num_t in4)
{
    stepper_motor_handle new_stepper = (stepper_motor_handle)pvPortMalloc(sizeof(stepper_motor_t));
    if(new_stepper == NULL)
    {
        ESP_LOGE(TAG, "pvPortMalloc Failed, exiting.");
        exit(EXIT_FAILURE);
    }
    gpio_reset_pin(in1);
    gpio_reset_pin(in2);
    gpio_reset_pin(in3);
    gpio_reset_pin(in4);

    gpio_set_direction(in1, GPIO_MODE_OUTPUT);
    const gpio_config_t config = {

        .pin_bit_mask = (1ULL << in1) | (1ULL << in2) | (1ULL << in3) | (1ULL << in4),
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&config));
    

    new_stepper->in1 = in1;
    new_stepper->in2 = in2;
    new_stepper->in3 = in3;
    new_stepper->in4 = in4;
    new_stepper->current_phase = 0;
    new_stepper->last_step_time = 0;
    new_stepper->target_steps = 0;
    new_stepper->steps_taken = 0;
    new_stepper->direction = STPR_MTR_NONE;
    new_stepper->ready = true;
    new_stepper->position = STPR_MTR_UNKNOWN;

    return new_stepper;
}

void stepper_motor_move(stepper_motor_handle stepper, const STPR_MTR_DIRECTION direction, const int16_t target_steps)
{
    stepper->target_steps = target_steps;
    stepper->direction = direction;
}

void stepper_motor_update(stepper_motor_handle stepper)
{
    int64_t current_time = esp_timer_get_time();
    static const int total_steps = 4;

    if(stepper->direction == STPR_MTR_NONE)
        return;

    if(current_time - stepper->last_step_time >= 10)
    {
        stepper->last_step_time = current_time;
        stepper->ready = false;

        if(stepper->direction == STPR_MTR_CW)
        {
            stepper->position = STPR_MTR_OPEN;
            stepper->current_phase = (stepper->current_phase + 1) % total_steps;
        }
        else if(stepper->direction == STPR_MTR_CCW)
        {
            stepper->position = STPR_MTR_CLOSED;
            stepper->current_phase = (stepper->current_phase + total_steps - 1) % total_steps;
        }
        else
        {
            ESP_LOGE(TAG, "Undefined motor direction.");
            exit(EXIT_FAILURE);
        }

        gpio_set_level(stepper->in1, full_step_wave[stepper->current_phase][0]);
        gpio_set_level(stepper->in2, full_step_wave[stepper->current_phase][1]);
        gpio_set_level(stepper->in3, full_step_wave[stepper->current_phase][2]);
        gpio_set_level(stepper->in4, full_step_wave[stepper->current_phase][3]);

        stepper->steps_taken++;

        if(stepper->steps_taken >= stepper->target_steps) // när sekvensen är färdig
        {
            stepper->direction  = STPR_MTR_NONE;
            stepper->target_steps = 0;
            stepper->steps_taken = 0;
            stepper->ready = true;
        }
    }
}

bool stepper_motor_ready(const stepper_motor_handle stepper)
{
    return stepper->ready;
}

STPR_MTR_POSITION get_stepper_motor_position(const stepper_motor_handle stepper)
{
    return stepper->position;
}