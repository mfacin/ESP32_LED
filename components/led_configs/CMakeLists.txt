idf_component_register(SRCS "led_effects.c" "ledc_config.c"
                      INCLUDE_DIRS "include"
                      REQUIRES driver freertos color_utils)