#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "hsv_to_rgb.h"
#include "ledc_config.h"
#include "led_effects.h"
#include "utils.h"
#include "colors.h"
#include "app_config.h"
#include "state_manager.h"
#include "effect_handlers.h"
#include "wifi_config.h"
#include "http_server.h"

/// @brief Estrutura para armazenamento dos delays dos efeitos.
typedef struct {
	int max_delay;
	int min_delay;
} effect_delays_t;

/// @brief Dicionário de delays máximos e mínimos para cada tipo de efeito.
static const effect_delays_t g_effect_delays[] = {
	[EFFECT_STATIONARY] = {STATIONARY_DELAY, STATIONARY_DELAY},
    [EFFECT_FADE]       = {MAX_FADE_DELAY, MIN_FADE_DELAY},
    [EFFECT_STROBE]     = {MAX_STROBE_DELAY, MIN_STROBE_DELAY},
    [EFFECT_BREATH]     = {MAX_BREATH_DELAY, MIN_BREATH_DELAY},
};

/**
 * @brief Task principal do FreeRTOS, responsável por controlar os efeitos e cores atuais do LED.
 */
void main_loop_task (void *param) {
	int delay_ms = 0;
	app_state_t *current_state = state_manager_get_state_pointer();

	while (true) {
		if (state_manager_lock_mutex() == pdTRUE) {	
			if (current_state->status == OFF) {
				set_color(hsv_to_rgb(BLACK_HSV));
				state_manager_unlock_mutex();
				vTaskDelay(pdMS_TO_TICKS(STATIONARY_DELAY));
				continue;
			}

			if (current_state->current_color.v > current_state->max_value) {
				current_state->current_color.v = current_state->max_value;
			}
			rgb_color_t selected_color = hsv_to_rgb(current_state->current_color);
			rgb_color_t final_color = correct_led_gamma(selected_color, current_state->gamma_correction.gamma_r, current_state->gamma_correction.gamma_g, current_state->gamma_correction.gamma_b);
			set_color(final_color);
			
			effect_type_t effect_type = current_state->current_effect.type;
			const effect_delays_t *delays = &g_effect_delays[effect_type];
			
			if (effect_type == EFFECT_STATIONARY) {
				delay_ms = delays->max_delay;
			} else {
				delay_ms = interpolate_speed_to_delay(current_state->current_effect.speed, delays->max_delay, delays->min_delay);
			}
			
			switch (current_state->current_effect.type) {
				case EFFECT_FADE:
				effect_fade(current_state);
				break;
				case EFFECT_STROBE:
				effect_strobe(current_state);
				break;
				case EFFECT_BREATH:
				effect_breath(current_state);
				break;	
				default:
				break;
			}
			
			state_manager_unlock_mutex();
		}

		vTaskDelay(pdMS_TO_TICKS(delay_ms));
	}
}

/**
 * @brief Função de entrada da aplicação.
 */
void app_main (void) {
	// Inicialização da NVS para Wi-Fi e state_manager.
	esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ESP_ERROR_CHECK(nvs_flash_init());
    }

	gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
	gpio_set_pull_mode(GPIO_NUM_0, GPIO_PULLUP_ONLY);
	vTaskDelay(pdMS_TO_TICKS(100));

	// Caso o botão de BOOT esteja acionado na inicialização, ele apaga a NVS.
	if (gpio_get_level(GPIO_NUM_0) == 0) {
		ESP_LOGI("MAIN", "Apagando a NVS");
		ESP_ERROR_CHECK(nvs_flash_erase());
	}

	srand(time(NULL));
	configure_ledc();
	state_manager_init();

	// Prioridade 4 garante que o HTTP tenha maior prioridade (5)
	xTaskCreate(main_loop_task, "main_loop_task", 4096, NULL, 4, NULL);

	wifi_init_station();
	http_server_start();
}