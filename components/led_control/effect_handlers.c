#include "effect_handlers.h"
#include "state_manager.h"

void handle_effect_change_command (effect_type_t effect_type, unsigned char speed) {
	effect_t new_effect = {
		.type = effect_type,
		.speed = (speed > 100 ? 100 : speed),
		.direction = BREATH_UP
	};

	state_manager_set_effect(&new_effect);
}

void handle_brightness_change_command (float value) {
	state_manager_set_brightness(value);
}

void handle_color_change_command (float hue, float saturation) {
	state_manager_set_hue_saturation(hue, saturation);
}

void handle_gamma_config_change_command (float gamma_r, float gamma_g, float gamma_b) {
	gamma_correction_t gamma_correction = {
		.gamma_r = gamma_r,
		.gamma_g = gamma_g,
		.gamma_b = gamma_b,
	};

	state_manager_set_gamma_config(&gamma_correction);
}