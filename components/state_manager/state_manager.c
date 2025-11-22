#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "state_manager.h"
#include "app_config.h"

#define STATE_TAG	"STATE_MANAGER"

/// @brief Estado atual da aplicação, especificando a cor, efeito e brilho definidos.
static app_state_t g_current_state;

/// @brief Mutex do estado da aplicação.
static SemaphoreHandle_t g_app_state_mutex;

/**
 * @brief Obtém um valor da NVS.
 * @param key Chave.
 * @param default_value Valor padrão caso a chave seja nula.
 */
static uint16_t get_value_from_nvs (const char *key, uint16_t default_value) {
	nvs_handle_t nvs_handle;
	uint16_t value = default_value;
	
	ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle));
	nvs_get_u16(nvs_handle, key, &value);
	nvs_close(nvs_handle);
	
	ESP_LOGI(STATE_TAG, "Got value from NVS: %s = %d", key, value);
	return value;
}

/**
 * @brief Salva um valor na NVS.
 * @param key Chave.
 * @param value Valor.
 */
static void save_value_to_nvs (const char *key, uint16_t value) {
	nvs_handle_t nvs_handle;

	ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle));
	nvs_set_u16(nvs_handle, key, value);
	nvs_commit(nvs_handle);
	nvs_close(nvs_handle);

	ESP_LOGI(STATE_TAG, "Set value to NVS: %s = %d", key, value);
	(void) get_value_from_nvs(key, 0);
}

/**
 * @brief Imprime os dados do estado especificado.
 * @param state Ponteiro para o estado a ser impresso.
 */
static void print_state (app_state_t *state) {
	ESP_LOGI(STATE_TAG, "Estado atual da aplicação:");
	ESP_LOGI(STATE_TAG, "\tcurrent_color = { h: %.2f, s: %.2f, v: %.2f }", state->current_color.h, state->current_color.s, state->current_color.v);
	ESP_LOGI(STATE_TAG, "\tcurrent_effect = { type: %d, speed: %d, direction: %d  }", state->current_effect.type, state->current_effect.speed, state->current_effect.direction);
	ESP_LOGI(STATE_TAG, "\tmax_value = %.2f", state->max_value);
	ESP_LOGI(STATE_TAG, "\tgamma_correction = { gamma_r: %.2f, gamma_g: %.2f, gamma_b: %.2f }", state->gamma_correction.gamma_r, state->gamma_correction.gamma_g, state->gamma_correction.gamma_b);
}

void state_manager_init (void) {
	g_app_state_mutex = xSemaphoreCreateMutex();

	effect_status_t status	= (effect_status_t) get_value_from_nvs(NVS_KEY_ONOFF, OFF);
	float hue			= (float) get_value_from_nvs(NVS_KEY_HUE, 0);
	float saturation	= (float) get_value_from_nvs(NVS_KEY_SATURATION, 100);
	float brightness	= (float) get_value_from_nvs(NVS_KEY_BRIGHTNESS, 75);
	uint16_t effect 	= get_value_from_nvs(NVS_KEY_EFFECT, EFFECT_STATIONARY);
	uint16_t speed 		= get_value_from_nvs(NVS_KEY_SPEED, 50);
	float gamma_r		= (float) get_value_from_nvs(NVS_KEY_GAMMA_R, RED_GAMMA_CORRECTION * 100);
	float gamma_g		= (float) get_value_from_nvs(NVS_KEY_GAMMA_G, GREEN_GAMMA_CORRECTION * 100);
	float gamma_b		= (float) get_value_from_nvs(NVS_KEY_GAMMA_B, BLUE_GAMMA_CORRECTION * 100);

	g_current_state = (app_state_t) {
		.status = status,
		.current_color = { .h = (hue / 100), .s = (saturation / 100), .v = (brightness / 100) },
		.current_effect = { .type = effect, .speed = speed, .direction = BREATH_UP },
		.max_value = (brightness / 100),
		.gamma_correction = { .gamma_r = (gamma_r / 100.0), .gamma_g = (gamma_g / 100.0), .gamma_b = (gamma_b / 100.0) }
	};

	print_state(&g_current_state);
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

		print_state(&g_current_state);
		save_value_to_nvs(NVS_KEY_ONOFF, (uint16_t) new_state->status);
		save_value_to_nvs(NVS_KEY_GAMMA_R, (uint16_t) (new_state->gamma_correction.gamma_r * 100));
		save_value_to_nvs(NVS_KEY_GAMMA_G, (uint16_t) (new_state->gamma_correction.gamma_g * 100));
		save_value_to_nvs(NVS_KEY_GAMMA_B, (uint16_t) (new_state->gamma_correction.gamma_b * 100));
		save_value_to_nvs(NVS_KEY_EFFECT, (uint16_t) new_state->current_effect.type);
		save_value_to_nvs(NVS_KEY_SPEED, (uint16_t) new_state->current_effect.speed);
		save_value_to_nvs(NVS_KEY_HUE, (uint16_t) (new_state->current_color.h * 100));
		save_value_to_nvs(NVS_KEY_SATURATION, (uint16_t) (new_state->current_color.s * 100));
		save_value_to_nvs(NVS_KEY_BRIGHTNESS, (uint16_t) (new_state->current_color.v * 100));
	}
}

void state_manager_set_on_off (effect_status_t status) {
	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state.status = status;
		state_manager_unlock_mutex();

		save_value_to_nvs(NVS_KEY_ONOFF, (uint16_t) status);
	}
}

void state_manager_set_hue_saturation (float hue, float saturation) {
	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state.current_color.h = hue;
		g_current_state.current_color.s = saturation;
		// Também define o estado como estacionário e o brilho como padrão.
		g_current_state.current_effect.type = EFFECT_STATIONARY;
		g_current_state.current_color.v = g_current_state.max_value;
		state_manager_unlock_mutex();

		save_value_to_nvs(NVS_KEY_EFFECT, (uint16_t) EFFECT_STATIONARY);
		save_value_to_nvs(NVS_KEY_HUE, (uint16_t) (hue * 100));
		save_value_to_nvs(NVS_KEY_SATURATION, (uint16_t) (saturation * 100));
	}
}

void state_manager_set_brightness (float value) {
	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state.current_color.v = value;
		g_current_state.max_value = value;
		state_manager_unlock_mutex();
		
		save_value_to_nvs(NVS_KEY_BRIGHTNESS, (uint16_t) (value * 100));
	}
}

void state_manager_set_effect (const effect_t* new_effect) {
	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state.current_effect = *new_effect;
		// Também define a saturação e o brilho como padrão.
		g_current_state.current_color.s = 1.0;
		g_current_state.current_color.v = g_current_state.max_value;
		state_manager_unlock_mutex();

		save_value_to_nvs(NVS_KEY_EFFECT, (uint16_t) new_effect->type);
		save_value_to_nvs(NVS_KEY_SPEED, (uint16_t) new_effect->speed);
		save_value_to_nvs(NVS_KEY_SATURATION, 100);
	}
}

app_state_t* state_manager_get_state_pointer (void) {
	return &g_current_state;
}

void state_manager_set_gamma_config (gamma_correction_t* gamma_correction) {
	if (state_manager_lock_mutex() == pdTRUE) {
		g_current_state.gamma_correction = *gamma_correction;
		state_manager_unlock_mutex();
		
		save_value_to_nvs(NVS_KEY_GAMMA_R, (uint16_t) (gamma_correction->gamma_r * 100));
		save_value_to_nvs(NVS_KEY_GAMMA_G, (uint16_t) (gamma_correction->gamma_g * 100));
		save_value_to_nvs(NVS_KEY_GAMMA_B, (uint16_t) (gamma_correction->gamma_b * 100));
	}
}
