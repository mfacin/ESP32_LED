#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_http_server.h"

#include "effect_handlers.h"

/// @brief Tag para os logs.
#define HTTP_TAG "HTTP_SERVER"

#define BODY_BUFFER_LEN		(128)
#define PARAM_BUFFER_LEN	(16)
#define OK_MESSAGE	"Ok"
#define ERROR_MSG	"Verifique que todos os parâmetros foram enviados"

/// @brief Estrutura para definição de parâmetros HTTP.
typedef struct {
	const char* key;	/**< Chave do parâmetro (nome) */
	char* buffer;		/**< Ponteiro para o buffer de destino */
	size_t buffer_len;	/**< Tamanho do buffer de destino */
} param_spec_t;

extern const uint8_t control_html_start[]	asm("_binary_control_html_start");
extern const uint8_t control_html_end[]		asm("_binary_control_html_end");

/**
 * @brief Copia o corpo de uma requisição HTTP POST para o buffer especificado.
 * @brief Envia uma resposta HTTP de erro caso não haja sucesso, ou caso o corpo seja nulo.
 * @param req Ponteiro da requisição de onde os parâmetros serão extraídos.
 * @param [out] buffer Buffer para onde os parâmetros serão copiados.
 * @param buffer_len Tamanho do buffer de cópia.
 * @return ESP_OK, se sucesso. ESP_FAIL, se erro.
 */
static esp_err_t http_get_post_body (httpd_req_t *req, char *buffer, size_t buffer_len) {
	int bytes_len = httpd_req_recv(req, buffer, buffer_len - 1);
	if (bytes_len <= 0) {
		ESP_LOGW(HTTP_TAG, "\tErro na leitura do corpo, ou requsição vazia.");
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ERROR_MSG);
		return ESP_FAIL;
	}
	
	buffer[bytes_len] = '\0'; // Adiciona o terminador nulo ao final do buffer.
	ESP_LOGI(HTTP_TAG, "\tCorpo: %s", buffer);
	return ESP_OK;
}

/**
 * @brief Extrai os parâmetros especificados de uma query string.
 * @param query_buffer A query string de onde os parâmetros serão extraídos.
 * @param params Array com as especificações dos parâmetros a serem extraídos.
 * @param num_params Número de parâmetros a serem extraídos.
 * @return ESP_OK, se sucesso. ESP_FAIL, se erro.
 */
static esp_err_t http_extract_params (const char *query_buffer, param_spec_t params[], size_t num_params) {
	esp_err_t status = ESP_OK;

	for (int i = 0; i < num_params; i++) {
		int ret = httpd_query_key_value(query_buffer, params[i].key, params[i].buffer, params[i].buffer_len);
		if (ret != ESP_OK) {
			ESP_LOGW(HTTP_TAG, "\tParâmetro %s não encontrado.", params[i].key);
			status = ESP_FAIL;
		}
	}

	return status;
}

/// @brief Handler para requisição no endpoint /set_on_off/.
/// @note Parâmetros = status.
static esp_err_t http_on_off_handler (httpd_req_t *req) {
	ESP_LOGI(HTTP_TAG, "Requisição recebida em /set_on_off");
	
	char body_buffer[BODY_BUFFER_LEN];
	char param_status[PARAM_BUFFER_LEN];

	param_spec_t params[] = {
		{ .key = "status", .buffer = param_status, .buffer_len = sizeof(param_status) },
	};
	
	// Extração do corpo da requisição.
	if (http_get_post_body(req, body_buffer, sizeof(body_buffer)) != ESP_OK) {
		return ESP_FAIL;
	}

	// Extração dos parâmetros.
	if (http_extract_params(body_buffer, params, 1)) {
		ESP_LOGW(HTTP_TAG, "\tRequisição com parâmetros insuficientes.");
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ERROR_MSG);
		return ESP_FAIL;
	}

	handle_on_off_change_command(atoi(param_status));

	httpd_resp_send(req, OK_MESSAGE, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

/// @brief Handler para requisição no endpoint /set_effect/.
/// @note Parâmetros = effect, speed.
static esp_err_t http_effect_handler (httpd_req_t *req) {
	ESP_LOGI(HTTP_TAG, "Requisição recebida em /set_effect");
	
	char body_buffer[BODY_BUFFER_LEN];
	char param_effect[PARAM_BUFFER_LEN];
	char param_speed[PARAM_BUFFER_LEN];

	param_spec_t params[] = {
		{ .key = "effect", .buffer = param_effect, .buffer_len = sizeof(param_effect) },
		{ .key = "speed", .buffer = param_speed, .buffer_len = sizeof(param_speed) },
	};
	
	// Extração do corpo da requisição.
	if (http_get_post_body(req, body_buffer, sizeof(body_buffer)) != ESP_OK) {
		return ESP_FAIL;
	}

	// Extração dos parâmetros.
	if (http_extract_params(body_buffer, params, 2)) {
		ESP_LOGW(HTTP_TAG, "\tRequisição com parâmetros insuficientes.");
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ERROR_MSG);
		return ESP_FAIL;
	}

	handle_effect_change_command(atoi(param_effect), (unsigned char) atoi(param_speed));

	httpd_resp_send(req, OK_MESSAGE, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

/// @brief Handler para requisição no endpoint /set_hue_saturation/.
/// @note Parâmetros = hue, saturation.
static esp_err_t http_hue_saturation_handler (httpd_req_t *req) {
	ESP_LOGI(HTTP_TAG, "Requisição recebida em /set_hue_saturation");

	char body_buffer[BODY_BUFFER_LEN];
	char param_hue[PARAM_BUFFER_LEN];
	char param_saturation[PARAM_BUFFER_LEN];

	param_spec_t params[] = {
		{ .key = "hue", .buffer = param_hue, .buffer_len = sizeof(param_hue) },
		{ .key = "saturation", .buffer = param_saturation, .buffer_len = sizeof(param_saturation) },
	};
	
	// Extração do corpo da requisição.
	if (http_get_post_body(req, body_buffer, sizeof(body_buffer)) != ESP_OK) {
		return ESP_FAIL;
	}

	// Extração dos parâmetros.
	if (http_extract_params(body_buffer, params, 2)) {
		ESP_LOGW(HTTP_TAG, "\tRequisição com parâmetros insuficientes.");
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ERROR_MSG);
		return ESP_FAIL;
	}

	handle_color_change_command(atof(param_hue), atof(param_saturation) / 100);

	httpd_resp_send(req, OK_MESSAGE, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

/// @brief Handler para requisição no endpoint /set_brightness/.
/// @note Parâmetro = brightness.
static esp_err_t http_brightness_handler (httpd_req_t *req) {
	ESP_LOGI(HTTP_TAG, "Requisição recebida em /set_brightness");

	char body_buffer[BODY_BUFFER_LEN];
	char param_brightness[PARAM_BUFFER_LEN];

	param_spec_t params[] = {
		{ .key = "brightness", .buffer = param_brightness, .buffer_len = sizeof(param_brightness) },
	};
	
	// Extração do corpo da requisição.
	if (http_get_post_body(req, body_buffer, sizeof(body_buffer)) != ESP_OK) {
		return ESP_FAIL;
	}

	// Extração dos parâmetros.
	if (http_extract_params(body_buffer, params, 1)) {
		ESP_LOGW(HTTP_TAG, "\tRequisição com parâmetros insuficientes.");
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ERROR_MSG);
		return ESP_FAIL;
	}

	handle_brightness_change_command(atof(param_brightness) / 100);

	httpd_resp_send(req, OK_MESSAGE, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

/// @brief Handler para requisição no endpoint /set_gamma_correction/.
/// @note Parâmetros = gammaR, gammaG, gammaB.
static esp_err_t http_gamma_correction_handler (httpd_req_t *req) {
	ESP_LOGI(HTTP_TAG, "Requisição recebida em /set_gamma_correction");

	char body_buffer[BODY_BUFFER_LEN];
	char param_gamma_r[PARAM_BUFFER_LEN];
	char param_gamma_g[PARAM_BUFFER_LEN];
	char param_gamma_b[PARAM_BUFFER_LEN];

	param_spec_t params[] = {
		{ .key = "gammaR", .buffer = param_gamma_r, .buffer_len = sizeof(param_gamma_r) },
		{ .key = "gammaG", .buffer = param_gamma_g, .buffer_len = sizeof(param_gamma_g) },
		{ .key = "gammaB", .buffer = param_gamma_b, .buffer_len = sizeof(param_gamma_b) }
	};
	
	// Extração do corpo da requisição.
	if (http_get_post_body(req, body_buffer, sizeof(body_buffer)) != ESP_OK) {
		return ESP_FAIL;
	}

	// Extração dos parâmetros.
	if (http_extract_params(body_buffer, params, 3)) {
		ESP_LOGW(HTTP_TAG, "\tRequisição com parâmetros insuficientes.");
		httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, ERROR_MSG);
		return ESP_FAIL;
	}

	handle_gamma_config_change_command(atof(param_gamma_r) / 100, atof(param_gamma_g) / 100, atof(param_gamma_b) / 100);

	httpd_resp_send(req, OK_MESSAGE, HTTPD_RESP_USE_STRLEN);
	return ESP_OK;
}

/// @brief Handler para requisição no endpoint /.
/// @note Retorna a página de controle da aplicação.
static esp_err_t root_get_handler (httpd_req_t *req) {
	ESP_LOGI(HTTP_TAG, "Requisição recebida em /");

	// Resposta com a página de controle da aplicação.
	const size_t control_html_len = control_html_end - control_html_start;
	httpd_resp_send(req, (const char*) control_html_start, control_html_len);

	return ESP_OK;
}

/// @brief Estrutura do handler da URI /set_on_off/.
static const httpd_uri_t on_off_uri = {
	.uri		= "/set_on_off",
	.method		= HTTP_POST,
	.handler	= http_on_off_handler,
	.user_ctx	= NULL
};

/// @brief Estrutura do handler da URI /set_effect/.
static const httpd_uri_t effect_uri = {
	.uri		= "/set_effect",
	.method		= HTTP_POST,
	.handler	= http_effect_handler,
	.user_ctx	= NULL
};

/// @brief Estrutura do handler da URI /set_hue_saturation/.
static const httpd_uri_t hue_saturation_uri = {
	.uri		= "/set_hue_saturation",
	.method		= HTTP_POST,
	.handler	= http_hue_saturation_handler,
	.user_ctx	= NULL
};

/// @brief Estrutura do handler da URI /set_brightness/.
static const httpd_uri_t brightness_uri = {
	.uri		= "/set_brightness",
	.method		= HTTP_POST,
	.handler	= http_brightness_handler,
	.user_ctx	= NULL
};

/// @brief Estrutura do handler da URI /set_gamma_correction/.
static const httpd_uri_t gamma_correction_uri = {
	.uri		= "/set_gamma_correction",
	.method		= HTTP_POST,
	.handler	= http_gamma_correction_handler,
	.user_ctx	= NULL
};

/// @brief Estrutura do handler da URI /.
static const httpd_uri_t root_uri = {
	.uri		= "/",
	.method		= HTTP_GET,
	.handler	= root_get_handler,
	.user_ctx	= NULL
};

httpd_handle_t http_server_start(void) {
	httpd_handle_t server = NULL;
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	config.lru_purge_enable = true; // Permite limpar conexões menos recentes

	ESP_LOGI(HTTP_TAG, "Iniciando servidor na porta: %d", config.server_port);
	if (httpd_start(&server, &config) == ESP_OK) {
		httpd_register_uri_handler(server, &on_off_uri);
		httpd_register_uri_handler(server, &effect_uri);
		httpd_register_uri_handler(server, &hue_saturation_uri);
		httpd_register_uri_handler(server, &brightness_uri);
		httpd_register_uri_handler(server, &gamma_correction_uri);
		httpd_register_uri_handler(server, &root_uri);
		return server;
	}

	ESP_LOGE(HTTP_TAG, "Erro ao iniciar o servidor HTTP");
	return NULL;
}