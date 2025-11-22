#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include "esp_err.h"

/**
 * @brief Copia o endereço IP atual para um ponteiro de destino.
 * @param[out] ip_out Ponteiro para o buffer de destino do IP.
 * @param buffer_size Tamanho do buffer de destino.
 * @return ESP_OK se a cópia for bem-sucedida. ESP_ERR_INVALID_ARG se o buffer de destino for de tamanho insuficiente.
 */
esp_err_t wifi_copy_ip_addrees (char* ip_out, size_t buffer_size);

/**
 * @brief Inicializa o Wi-Fi no modo Estação (Cliente)
 */
void wifi_init_station (void);

#endif