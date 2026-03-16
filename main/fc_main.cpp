#include <esp_log.h>
#include "sign_flipper.hpp"
#include "pin_defs.hpp"

#define TAG "main"

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Lay hou!");
    auto &flipper = sign_flipper::instance();
    ESP_ERROR_CHECK(flipper.init());


    while (true) {
        flipper.open();
        flipper.wait_till_idle(pdMS_TO_TICKS(1000));
        vTaskDelay(pdMS_TO_TICKS(1000));
        flipper.close();
        flipper.wait_till_idle(pdMS_TO_TICKS(1000));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
