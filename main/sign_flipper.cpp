#include <driver/gpio.h>

#include "pin_defs.hpp"
#include "sign_flipper.hpp"

esp_err_t sign_flipper::init()
{
    state = xEventGroupCreate();
    if (state == nullptr) {
        ESP_LOGE(TAG, "init: can't initialise state group");
        return ESP_ERR_NO_MEM;
    }

    gpio_install_isr_service(0);
    esp_err_t ret = gpio_reset_pin(flipcab::LED_CTRL);
    ret = ret ?: gpio_reset_pin(flipcab::MOTOR_CTRL_1);
    ret = ret ?: gpio_reset_pin(flipcab::MOTOR_CTRL_2);

    gpio_config_t motor_cfg = {};
    motor_cfg.intr_type = GPIO_INTR_DISABLE;
    motor_cfg.mode = GPIO_MODE_OUTPUT;
    motor_cfg.pin_bit_mask = ( 1 << flipcab::MOTOR_CTRL_1 ) | ( 1 << flipcab::MOTOR_CTRL_2 ) | ( 1 << flipcab::LED_CTRL );
    motor_cfg.pull_down_en = GPIO_PULLDOWN_ENABLE;
    motor_cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    ret = ret ?: gpio_config(&motor_cfg);
    ret = ret ?: gpio_set_level(flipcab::MOTOR_CTRL_1, 0);
    ret = ret ?: gpio_set_level(flipcab::MOTOR_CTRL_2, 0);
    ret = ret ?: gpio_set_level(flipcab::LED_CTRL, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "init: failed to set motor GPIO!");
        return ret;
    }

    gpio_config_t limit_switch_cfg = {};
    limit_switch_cfg.mode = GPIO_MODE_INPUT;
    limit_switch_cfg.intr_type = GPIO_INTR_NEGEDGE;
    limit_switch_cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    limit_switch_cfg.pull_up_en = GPIO_PULLUP_ENABLE;
    limit_switch_cfg.pin_bit_mask = ( 1 << flipcab::LIMIT_SWITCH_CLOSE ) | ( 1 << flipcab::LIMIT_SWITCH_OPEN );
    ret = gpio_config(&limit_switch_cfg);
    ret = ret ?: gpio_isr_handler_add(flipcab::LIMIT_SWITCH_OPEN, limit_sw_open_isr, this);
    ret = ret ?: gpio_isr_handler_add(flipcab::LIMIT_SWITCH_CLOSE, limit_sw_close_isr, this);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "init: failed to set limit switch GPIO!");
        return ret;
    }

    if (BaseType_t task_ret = xTaskCreate(flipper_task, "flipper_task", 8192, this, tskIDLE_PRIORITY + 3, &task); task_ret != pdPASS) {
        ESP_LOGE(TAG, "init: failed to create task!");
        return task_ret;
    }

    return ESP_OK;
}

void sign_flipper::flipper_task(void* _ctx)
{
}

void sign_flipper::limit_sw_close_isr(void* _ctx)
{
    auto *ctx = static_cast<sign_flipper *>(_ctx);
    BaseType_t woken = pdFALSE;
    xEventGroupSetBitsFromISR(ctx->state, STATE_HIT_CLOSE, &woken);
    if (woken) {
        portYIELD_FROM_ISR();
    }
}

void sign_flipper::limit_sw_open_isr(void* _ctx)
{
    auto *ctx = static_cast<sign_flipper *>(_ctx);
    BaseType_t woken = pdFALSE;
    xEventGroupSetBitsFromISR(ctx->state, STATE_HIT_OPEN, &woken);
    if (woken) {
        portYIELD_FROM_ISR();
    }
}
