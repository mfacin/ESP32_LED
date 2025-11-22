#include <stdbool.h>
#include <math.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ledc_config.h"
#include "led_effects.h"
#include "led_control.h"
#include "colors.h"
#include "app_config.h"

void stationary_color (app_state_t *current_state, hsv_color_t color) {
	current_state->current_color = color;
}

void effect_fade (app_state_t *current_state) {
	// "Gira" a matiz de grau em grau
	current_state->current_color.h = fmod(current_state->current_color.h + 1, 360);
}

void effect_strobe (app_state_t *current_state) {
	// "Gira" a matiz em 60º
	// (int) (floor(current_state->current_color.h / 60) * 60) garante que será somente de 60 em 60
	current_state->current_color.h = fmod((int) (floor(current_state->current_color.h / 60) * 60) + 60, 360);
}

void effect_breath(app_state_t *current_state) {
	// Garantindo que a cor esteja em algum setor de 60 em 60
	current_state->current_color.h = fmod((int) (floor(current_state->current_color.h / 60) * 60), 360);
	
	float step_base = (float) current_state->current_effect.direction / (float) BREATH_EFFECT_STEPS;
	// Multiplicar pelo max_value permite que o efeito mantenha a mesma velocidade
	current_state->current_color.v += step_base * current_state->max_value;

	if (current_state->current_color.v <= 0) {
		// Chegou no mínimo, muda a cor e a direção
		current_state->current_color.v = 0;
		current_state->current_color.h = fmod(current_state->current_color.h + 60, 360);
		current_state->current_effect.direction = BREATH_UP;
	} else if (current_state->current_color.v >= current_state->max_value) {
		// Chegou no máximo, muda a direção
		current_state->current_color.v = current_state->max_value;
		current_state->current_effect.direction = BREATH_DOWN;
	}	
}