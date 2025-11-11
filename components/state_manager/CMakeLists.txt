idf_component_register(SRCS "state_manager.c"
                      INCLUDE_DIRS "include"
                      REQUIRES freertos colors nvs_flash app_config)