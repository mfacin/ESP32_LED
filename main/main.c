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
 * @brief Configura os campos do estado da aplicação de acordo com o novo efeito a ser iniciado.
 * @param *current_state Ponteiro da estrutura de estado atual da aplicação.
 * @param effect_type O novo efeito a ser iniciado.
 * @param speed A velocidade do efeito.
 */
void start_effect (app_state_t *current_state, effect_type_t effect_type, char speed) {
	current_state->current_color.h = 0;
	current_state->current_color.s = 0;
	current_state->current_color.v = current_state->max_value;
	current_state->current_effect.type = effect_type;
	current_state->current_effect.speed = speed;
	current_state->current_effect.direction = BREATH_UP;
}

/**
 * @brief Task principal do FreeRTOS, responsável por controlar os efeitos e cores atuais do LED.
 */
void main_loop_task (void *param) {
	int delay_ms = 0;
	app_state_t *current_state = state_manager_get_state_pointer();

	while (true) {
		vTaskDelay(pdMS_TO_TICKS(delay_ms));

		if (state_manager_lock_mutex() == pdTRUE) {		
			if (current_state->current_color.v > current_state->max_value) {
				current_state->current_color.v = current_state->max_value;
			}
			rgb_color_t selected_color = hsv_to_rgb(current_state->current_color);
			rgb_color_t final_color = correct_led_gamma(selected_color, RED_GAMMA_CORRECTION, GREEN_GAMMA_CORRECTION, BLUE_GAMMA_CORRECTION);
			set_color(final_color);
		
			effect_type_t effect_type = current_state->current_effect.type;
			const effect_delays_t *delays = &g_effect_delays[effect_type];
		
			if (effect_type == EFFECT_STATIONARY) {
				delay_ms = delays->max_delay;
			} else {
				delay_ms = interpolate_speed_to_delay(current_state->current_effect.speed, delays->max_delay, delays->min_delay);
			}

			state_manager_lock_mutex();
		}
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
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

	srand(time(NULL));
	configure_ledc();
	state_manager_init();

	xTaskCreate(main_loop_task, "main_loop_task", 4096, NULL, 5, NULL);
}