#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "colors.h"
#include "ledc_config.h"

/**
 * @brief Configura os LEDs para a cor RGB especificada.
 * @param rgb_color A cor a ser definida no LED.
 * @note Essa função realiza a correção do brilho máximo e do gama de cada componente.
 */
void set_color (rgb_color_t rgb_color);

/**
 * @brief Atualiza o duty cicle do PWM do canal especificado.
 * @param channel O canal que terá o duty cicle atualizado.
 * @param duty O novo valor do duty cicle.
 */
void update_duty (ledc_channel_t channel, int duty);

#endif