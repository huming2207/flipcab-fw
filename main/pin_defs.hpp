#pragma once

#include <driver/gpio.h>

namespace flipcab
{
    static constexpr gpio_num_t LIMIT_SWITCH_CLOSE = GPIO_NUM_2;
    static constexpr gpio_num_t LIMIT_SWITCH_OPEN = GPIO_NUM_3;
    static constexpr gpio_num_t LED_CTRL = GPIO_NUM_14;
    static constexpr gpio_num_t MOTOR_CTRL_1 = GPIO_NUM_20;
    static constexpr gpio_num_t MOTOR_CTRL_2 = GPIO_NUM_21;
}