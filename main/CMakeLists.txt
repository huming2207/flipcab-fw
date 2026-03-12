idf_component_register(SRCS "fc_main.cpp" "sign_flipper.cpp"
                       PRIV_REQUIRES driver esp_driver_gpio nvs_flash
                       INCLUDE_DIRS ".")
