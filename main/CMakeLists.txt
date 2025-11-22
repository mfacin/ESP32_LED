idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES led_control colors utils app_config nvs_flash http_server wifi_config)