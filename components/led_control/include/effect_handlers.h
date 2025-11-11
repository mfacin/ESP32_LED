#ifndef EFFECT_CONTROL_H
#define EFFECT_CONTROL_H

#include "led_effects.h"

/**
 * @brief Função para manipulação de mudança de efeito, a ser chamada pelas APIs.
 * @param effect_type Tipo de efeito a ser definido.
 * @param speed Velocidade do efeito.
 */
void handle_effect_change_command (effect_type_t effect_type, unsigned char speed);

/**
 * @brief Função para manipulação de mudança de brilho, a ser chamada pelas APIs.
 * @param value O novo valor do brilho.
 */
void handle_brightness_change_command (float value);

/**
 * @brief Função para manipulação de mudança de cor, a ser chamada pelas APIs.
 * @param hue Novo valor do hue.
 * @param saturation Novo valor da saturação.
 */
void handle_color_change_command (float hue, float saturation);

/**
 * @brief Função para manipulação de mudança dos fatores de correção de gama, a ser chamada pelas APIs.
 * @param gamma_r Novo valor do fator de correção do LED vermelho.
 * @param gamma_g Novo valor do fator de correção do LED verde.
 * @param gamma_b Novo valor do fator de correção do LED azul.
 */
void handle_gamma_config_change_command (float gamma_r, float gamma_g, float gamma_b);

#endif