#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "colors.h"

/// @brief Quantidade de passos que o efeito breath dará em uma direção.
#define BREATH_EFFECT_STEPS	(100.0)

/// @brief Delay para cor estacionária, apenas para controle no update().
#define STATIONARY_DELAY	(1000)

/// @brief Delay mínimo, em ms, do efeito de fade. Em suma, a velocidade máxima do efeito.
#define MIN_FADE_DELAY		(1)
/// @brief Delay mínimo, em ms, do efeito de strobe. Em suma, a velocidade máxima do efeito.
#define MIN_STROBE_DELAY	(100)
/// @brief Delay mínimo, em ms, do efeito de breath. Em suma, a velocidade máxima do efeito.
#define MIN_BREATH_DELAY	(1)

/// @brief Delay máximo, em ms, do efeito de fade. Em suma, a velocidade mínima do efeito.
#define MAX_FADE_DELAY		(100)
/// @brief Delay máximo, em ms, do efeito de strobe. Em suma, a velocidade mínima do efeito.
#define MAX_STROBE_DELAY	(5000)
/// @brief Delay máximo, em ms, do efeito de breath. Em suma, a velocidade mínima do efeito.
#define MAX_BREATH_DELAY	(100)

/// @brief Fator de correção do gama vermelho
#define RED_GAMMA_CORRECTION	(1.0)
/// @brief Fator de correção do gama verde
#define GREEN_GAMMA_CORRECTION	(0.5)
/// @brief Fator de correção do gama azul
#define BLUE_GAMMA_CORRECTION	(1.0)

/**
 * @brief Tipos de efeitos disponíveis.
 */
typedef enum { EFFECT_STATIONARY, EFFECT_FADE, EFFECT_STROBE, EFFECT_BREATH } effect_type_t;

/**
 * @brief Direção do efeito de breath.
 * @note BREATH_UP = aumentando o brilho;
 * @note BREATH_DOWN = diminuindo o brilho.
 */
typedef enum { BREATH_UP = 1, BREATH_DOWN = -1 } effect_breath_direction_t;

/**
 * @brief Configuração de efeitos.
 */
typedef struct {
	effect_type_t type; /**< Tipo de efeito. */
	unsigned char speed; /**< Velocidade para mudanças de cores (0-100%). */
	effect_breath_direction_t direction; /**< Direção do efeito (-1, 1). Utilizado no breath. */
} effect_t;

/**
 * @brief Configuração do estado atual da aplicação, indicando a cor, efeito, e brilho máximo.
 */
typedef struct {
	hsv_color_t current_color; /**< Cor atual. */
	effect_t current_effect; /**< Efeito atual. */
	float max_value; /**< Brilho máximo (componente value). */
} app_state_t;

#endif