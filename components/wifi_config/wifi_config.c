#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_log.h"

#include "wifi_config.h"
#include "app_config.h"

/// @brief Tag para uso nos logs.
#define WIFI_TAG "WIFI_CONFIG"

/// @brief Endereço IP obtido pela rede.
static char current_ip_addr[16] = "0.0.0.0";

/**
 * @brief Manipulador de eventos de Wi-Fi e IP.
 */ 
static void event_handler (void* args, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	static int retry_attempts = 0;

	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		// Estação iniciada, seguiremos com a conexão ao Wi-Fi.
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		// Estação desconectada do AP, serão realizadas novas tentativas.
		if (retry_attempts < WIFI_MAX_RETRY) {
			esp_wifi_connect();
			ESP_LOGI(WIFI_TAG, "Tentando reconexão ao AP - %d/%d", retry_attempts, WIFI_MAX_RETRY);
			retry_attempts++;
		} else {
			ESP_LOGE(WIFI_TAG, "Falha ao conectar ao AP. Número máximo de tentativas atingido.");
		}
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		// Estação conectada ao AP e endereço IP recebido.
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		snprintf(current_ip_addr, sizeof(current_ip_addr), IPSTR, IP2STR(&event->ip_info.ip));
		ESP_LOGI(WIFI_TAG, "Conexão bem-sucedida. IP obtido: %s", current_ip_addr);
		retry_attempts = 0;
	}
}

esp_err_t wifi_copy_ip_addrees (char* ip_out, size_t buffer_size) {
	if (ip_out == NULL || buffer_size < sizeof(current_ip_addr)) {
		ESP_LOGE(WIFI_TAG, "Ponteiro ou buffer para cópia do IP são de tamanhos insuficientes");
		if (ip_out != NULL && buffer_size > 0) { ip_out[0] = '\0'; } // Limpa o buffer de saída.

		return ESP_ERR_INVALID_ARG;
	}

	snprintf(ip_out, buffer_size, "%s", current_ip_addr);
	return ESP_OK;
}

void wifi_init_station (void) {
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip));

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = WIFI_SSID,
			.password = WIFI_PASS,
			.threshold.authmode = WIFI_AUTH_WPA2_PSK,
		},
	};

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
	ESP_ERROR_CHECK(esp_wifi_start());

	ESP_LOGI(WIFI_TAG, "Conectando-se a %s.", WIFI_SSID);
}