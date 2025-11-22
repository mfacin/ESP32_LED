idf_component_register(SRCS "http_server.c"
                      INCLUDE_DIRS "include"
                      EMBED_FILES control.html
                      REQUIRES freertos app_config esp_event esp_netif esp_http_server led_control)