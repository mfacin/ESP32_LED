#ifndef LEDC_CONFIG_H
#define LEDC_CONFIG_H

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"

/// @brief Configuração de GPIO do LED vermelho.
#define RED_GPIO 	(25)
/// @brief Configuração de GPIO do LED verde.
#define GREEN_GPIO 	(26)
/// @brief Configuração de GPIO do LED azul.
#define BLUE_GPIO 	(27)

/// @brief Configuração de canal do LEDC do LED vermelho.
#define RED_CHANNEL		LEDC_CHANNEL_0
/// @brief Configuração de canal do LEDC do LED verde.
#define GREEN_CHANNEL	LEDC_CHANNEL_1
/// @brief Configuração de canal do LEDC do LED azul.
#define BLUE_CHANNEL	LEDC_CHANNEL_2

/// @brief Configuração de velocidade do LEDC.
#define SPEED_MODE	LEDC_LOW_SPEED_MODE
/// @brief Configuração de timer do LEDC.
#define TIMER		LEDC_TIMER_0
/// @brief Configuração de resolução de duty do LEDC.
#define DUTY_RES	LEDC_TIMER_8_BIT
/// @brief Configuração de frequeência do LEDC.
#define FREQ_HZ		(5000)

/**
 * @brief Configuração de canal do LEDC, associando um GPIO a um canal.
 */
void configure_ledc_channel (ledc_channel_t channel, int gpio_num);

/**
 * @brief Configuração geral do LEDC para a aplicação, aplicando os canais e timer.
 */
void configure_ledc ();

#endif