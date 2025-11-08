#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

#include "hsv_to_rgb.h"
#include "ledc_config.h"
#include "led_effects.h"
#include "utils.h"
#include "colors.h"
#include "app_config.h"

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

/// @brief Estado atual da aplicação, especificando a cor, efeito e brilho definidos.
static app_state_t g_current_state = {
	.current_color = { .h = 60, .s = 1, .v = 0.75 },
	.current_effect = {
		.type = EFFECT_STATIONARY,
		.speed = 0.5,
		.direction = BREATH_UP
	},
	.max_value = 0.75
};

/// @brief Mutex do estado da aplicação.
static SemaphoreHandle_t g_app_state_mutex;

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

	while (true) {
		vTaskDelay(pdMS_TO_TICKS(delay_ms));

		if (xSemaphoreTake(g_app_state_mutex, portMAX_DELAY) == pdTRUE) {
			app_state_t *current_state = (app_state_t *) param;
		
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

			xSemaphoreGive(g_app_state_mutex);
		}
	}
}

/**
 * @brief Função de entrada da aplicação.
 */
void app_main (void) {
	srand(time(NULL));
	configure_ledc();

	g_app_state_mutex = xSemaphoreCreateMutex();

	xTaskCreate(main_loop_task, "main_loop_task", 4096, &g_current_state, 5, NULL);
}