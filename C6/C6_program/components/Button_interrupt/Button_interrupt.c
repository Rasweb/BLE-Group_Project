#include <stdio.h>
#include "Button_interrupt.h"

static esp_err_t btn_i_register_isr(btn_i_handle button);


btn_i_handle btn_i_init(const gpio_num_t pin, const strapping_mode mode, const int debounce_ms)
{
    gpio_reset_pin(pin); //ny
    btn_i_handle button = (btn_i_handle)pvPortMalloc(sizeof(btn_i_conf));
    if (!button)
    {
        printf("Malloc Failed\n");
        exit(1);
    }
    int pull_down = -1;
    int pull_up = -1;
    switch (mode)
    {
    case PULLDOWN:
        pull_down = GPIO_PULLDOWN_ENABLE;
        pull_up = GPIO_PULLUP_DISABLE;
        break;

    case PULLUP:
        pull_down = GPIO_PULLDOWN_DISABLE;
        pull_up = GPIO_PULLUP_ENABLE;
        break;

    case NEITHER:
        pull_down = GPIO_PULLDOWN_DISABLE;
        pull_up = GPIO_PULLUP_DISABLE;
        break;

    default:
        printf("Invalid choice, exiting program!\n");
        exit(1);
    }

    gpio_config_t button_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 1ULL << pin,
        .pull_down_en = pull_down,
        .pull_up_en = pull_up,
    };
    ESP_ERROR_CHECK(gpio_config(&button_conf));

    button->pin = pin;
    button->flag = false;
    button->mode = mode;
    button->debounce_time = debounce_ms;
    button->counter = 0;
    button->interrupt_triggered = false;
    button->STATE = BTN_IDLE;
    button->last_trig_time = 0;

    ESP_ERROR_CHECK(btn_i_register_isr(button));

    return button;
}

static void IRAM_ATTR button_isr_handler(void *arg)
{
    btn_i_handle button = (btn_i_handle)arg;
    gpio_isr_handler_remove(button->pin);
    button->interrupt_triggered = true;
}

static esp_err_t btn_i_register_isr(btn_i_handle button)
{
    esp_err_t err_isr_register = gpio_install_isr_service(0);
    if (err_isr_register != ESP_OK && err_isr_register != ESP_ERR_INVALID_STATE)
    {
        printf("Error att installing isr service!\n");
        return err_isr_register;
    }
    return gpio_isr_handler_add(button->pin, button_isr_handler, (void *)button);
}

void btn_i_update(btn_i_handle button)
{
    int currentTime = xTaskGetTickCount();
    switch (button->STATE)
    {
    case BTN_IDLE:
        if (button->interrupt_triggered)
        {
            printf("Button Pressed ");
            button->flag = true; // för att göra hanteringen av tryckningen asynkront (i blinking_system), annars kan det missas då button_state förändras fort
            button->STATE = BTN_PRESSED;
            button->counter++;
            printf("Button Count: %d\n", button->counter);
            button->last_trig_time = currentTime;
            button->interrupt_triggered = false;
        }
        break;

    case BTN_PRESSED:
        if (gpio_get_level(button->pin) == 0)
        {
            button->STATE = BTN_RELEASED;
            button->interrupt_triggered = false;
            button->debounce_start_time = currentTime;
        }
        break;

    case BTN_RELEASED:
        if (currentTime - button->debounce_start_time >= pdMS_TO_TICKS(button->debounce_time))
        {
            button->STATE = BTN_IDLE;
            ESP_ERROR_CHECK(gpio_isr_handler_add(button->pin, button_isr_handler, button));
        }
        break;
    }
}

bool btn_pressed(const btn_i_handle button)
{
    return button->flag;
}

void btn_clear_flag(btn_i_handle button)
{
    button->flag = false;
}
