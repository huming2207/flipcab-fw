#pragma once

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>


class sign_flipper
{
public:
    static sign_flipper &instance()
    {
        static sign_flipper _instance;
        return _instance;
    }

    enum state_bits : uint32_t
    {
        STATE_EXEC_OPEN = BIT(0),
        STATE_EXEC_CLOSE = BIT(1),
        STATE_EXECUTE = STATE_EXEC_OPEN | STATE_EXEC_CLOSE,
        STATE_BRAKE = BIT(2),
        STATE_HIT_OPEN = BIT(8),
        STATE_HIT_CLOSE = BIT(9),
        STATE_HIT = STATE_HIT_OPEN | STATE_HIT_CLOSE,
    };

    void operator=(sign_flipper const &) = delete;
    sign_flipper(sign_flipper const &) = delete;

private:
    sign_flipper() = default;
    EventGroupHandle_t state = nullptr;
    TaskHandle_t task = nullptr;

public:
    esp_err_t init();
    void open() const;
    void close() const;

private:
    static constexpr char TAG[] = "flip_sign";
    static void flipper_task(void *_ctx);
    static void limit_sw_close_isr(void *_ctx);
    static void limit_sw_open_isr(void *_ctx);
};
