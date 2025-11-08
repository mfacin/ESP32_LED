idf_component_register(SRCS "led_control.c" "led_effects.c" "ledc_config.c"
                      INCLUDE_DIRS "include"
                      REQUIRES driver freertos colors app_config)