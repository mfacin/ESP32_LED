#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "nvs_flash.h"

#include "state_manager.h"
#include "app_config.h"

/// @brief Estado atual da aplicação, especificando a cor, efeito e brilho definidos.
static app_state_t g_current_state;

/// @brief Mutex do estado da aplicação.
static SemaphoreHandle_t g_app_state_mutex;

void state_manager_init (void) {
	g_app_state_mutex = xSemaphoreCreateMutex();

	nvs_handle_t nvs_handle;
	nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
	
	uint8_t gamma_r = RED_GAMMA_CORRECTION, gamma_g = GREEN_GAMMA_CORRECTION, gamma_b = BLUE_GAMMA_CORRECTION;
	nvs_get_u8(nvs_handle, NVS_KEY_GAMMA_R, &gamma_r);
	nvs_get_u8(nvs_handle, NVS_KEY_GAMMA_G, &gamma_g);
	nvs_get_u8(nvs_handle, NVS_KEY_GAMMA_B, &gamma_b);
	nvs_close(nvs_handle);

	g_current_state = (app_state_t) {
		.current_color = { .h = 60, .s = 1, .v = 0.75 },
		.current_effect = { .type = EFFECT_STATIONARY, .speed = 0.5, .direction = BREATH_UP	},
		.max_value = 0.75,
		.gamma_correction = { .gamma_r = (gamma_r / 100.0), .gamma_g = (gamma_g / 100.0), .gamma_b = (gamma_b / 100.0) }
	};
}

BaseType_t state_manager_lock_mutex (void) {
	return xSemaphoreTake(g_app_state_mutex, portMAX_DELAY);
}

BaseType_t state_manager_unlock_mutex (void) {
	return xSemaphoreGive(g_app_state_mutex);
}

void state_manager_get_copy (app_state_t* out_state) {
	if (state_manager_lock_mutex() == pdTRUE) {
		*out_state = g_current_state;
		state_manager_unlock_mutex();
	}
}

void state_manager_set_full_state (const app_state_t* new_state) {
	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state = *new_state;
		state_manager_unlock_mutex();
	}
}

void state_manager_set_hue_saturation (float hue, float saturation) {
	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state.current_color.h = hue;
		g_current_state.current_color.s = saturation;
		state_manager_unlock_mutex();
	}
}

void state_manager_set_brightness (float value) {
	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state.current_color.v = value;
		g_current_state.max_value = value;
		state_manager_unlock_mutex();
	}
}

void state_manager_set_effect (const effect_t* new_effect) {
	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state.current_effect = *new_effect;
		state_manager_unlock_mutex();
	}
}

app_state_t* state_manager_get_state_pointer (void) {
	return &g_current_state;
}

void state_manager_set_gamma_config (gamma_correction_t* gamma_correction) {
	if (gamma_correction->gamma_r < 0) { gamma_correction->gamma_r = 0;	}
	if (gamma_correction->gamma_r > 1) { gamma_correction->gamma_r = 1;	}
	if (gamma_correction->gamma_g < 0) { gamma_correction->gamma_g = 0;	}
	if (gamma_correction->gamma_g > 1) { gamma_correction->gamma_g = 1;	}
	if (gamma_correction->gamma_b < 0) { gamma_correction->gamma_b = 0;	}
	if (gamma_correction->gamma_b > 1) { gamma_correction->gamma_b = 1;	}

	nvs_handle_t nvs_handle;
    nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
	
	// A NVS não salva floats. Os valores serão convertidos para [0, 100].
    nvs_set_u8(nvs_handle, NVS_KEY_GAMMA_R, (uint8_t) (gamma_correction->gamma_r * 100));
    nvs_set_u8(nvs_handle, NVS_KEY_GAMMA_G, (uint8_t) (gamma_correction->gamma_g * 100));
    nvs_set_u8(nvs_handle, NVS_KEY_GAMMA_B, (uint8_t) (gamma_correction->gamma_b * 100));

    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);

	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state.gamma_correction = *gamma_correction;
		state_manager_unlock_mutex();
	}
}
