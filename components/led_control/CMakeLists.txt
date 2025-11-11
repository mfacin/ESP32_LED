idf_component_register(SRCS "effect_handlers.c" "led_control.c" "led_effects.c" "ledc_config.c"
                      INCLUDE_DIRS "include"
                      REQUIRES driver freertos colors app_config state_manager)