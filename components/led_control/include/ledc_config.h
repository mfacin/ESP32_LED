#ifndef LEDC_CONFIG_H
#define LEDC_CONFIG_H

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "app_config.h"


/**
 * @brief Configuração de canal do LEDC, associando um GPIO a um canal.
 */
void configure_ledc_channel (ledc_channel_t channel, int gpio_num);

/**
 * @brief Configuração geral do LEDC para a aplicação, aplicando os canais e timer.
 */
void configure_ledc (void);

#endif