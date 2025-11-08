#ifndef LED_EFFECTS_H
#define LED_EFFECTS_H

#include <stdbool.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_control.h"
#include "app_config.h"

/**
 * @brief Cor estacionária.
 * @param *current_state Ponteiro para a estrutura de estado atual da aplicação.
 * @param color Cor a ser definida.
 */
void stationary_color(app_state_t *current_state, hsv_color_t color);

/**
 * @brief Efeito de fade, onde as cores "viajam" pelo espectro RGB, circulando pela matiz.
 * @param *current_state Ponteiro para a estrutura de estado atual da aplicação.
 */
void effect_fade(app_state_t *current_state);

/***
 * @brief Efeito de strobe, onde as cores estacionárias alteram rapidamente, em um efeito estroboscópio.
 * @param *current_state Ponteiro para a estrutura de estado atual da aplicação.
 */
void effect_strobe (app_state_t *current_state);

/***
 * @brief Efeito de respiração, onde as cores alteram entre si, indo da menor até a maior luminosidade individualmente.
 * @param *current_state Ponteiro para a estrutura de estado atual da aplicação.
 */
void effect_breath(app_state_t *current_state);

#endif