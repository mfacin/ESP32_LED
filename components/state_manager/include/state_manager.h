#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "colors.h"

/**
 * @brief Status de ON/OFF.
 */
typedef enum { OFF, ON } effect_status_t;

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
 * @brief Configuração dos fatores de correção de gama dos LEDs.
 */
typedef struct {
	float gamma_r;
	float gamma_g;
	float gamma_b;
} gamma_correction_t;

/**
 * @brief Configuração do estado atual da aplicação, indicando a cor, efeito, e brilho máximo.
 */
typedef struct {
	effect_status_t status; /** Status de ON/OFF. */
	hsv_color_t current_color; /**< Cor atual. */
	effect_t current_effect; /**< Efeito atual. */
	float max_value; /**< Brilho máximo (componente value). */
	gamma_correction_t gamma_correction; /**< Fatores de correção de gama */
} app_state_t;

/**
 * @brief Inicialização do estado da aplicação.
 */
void state_manager_init (void);

/**
 * @brief Bloqueia a Mutex do estado da aplicação e obtém o semáforo da mesma.
 * @return pdTRUE se o semáforo foi obtido. pdFALSE se o timeout expirou sem a obtenção do semáforo.
 */
BaseType_t state_manager_lock_mutex (void);

/**
 * @brief Desbloqueia a Mutex do estado da aplicação e libera o semáforo da mesma.
 * @return pdTRUE se o semáforo foi liberado. pdFALSE se ocorreu um erro.
 */
BaseType_t state_manager_unlock_mutex (void);

/**
 * @brief Obtém uma cópia do estado atual da aplicação.
 * @param[out] out_state Ponteiro para uma struct local onde o estado será copiado.
 */
void state_manager_get_copy (app_state_t* out_state);

/**
 * @brief Altera o estado inteiro, definindo um novo efeito, cor e brilho máximo.
 * @param new_state Ponteiro para uma struct local com os dados do novo estado.
 */
void state_manager_set_full_state (const app_state_t* new_state);

/**
 * @brief Define o status de ON/OFF
 * @param status Novo status.
 */
void state_manager_set_on_off (effect_status_t status);

/**
 * @brief Define os valores de hue (matiz) e saturação
 * @param hue Novo valor do hue.
 * @param saturation Novo valor da saturação.
 */
void state_manager_set_hue_saturation (float hue, float saturation);

/**
 * @brief Define o valor do brilho máximo.
 * @param value Novo valor do brilho.
 */
void state_manager_set_brightness (float value);

/**
 * @brief Define um novo efeito.
 * @param new_effect Ponteiro para uma struct com os dados do novo efeito.
 */
void state_manager_set_effect (const effect_t* new_effect);

/**
 * @brief Obtém o ponteiro para o estado interno da aplicação.
 * @note Utilizado apenas pela função de feito da lógica principal.
 * @note Esta função NÃO utiliza do Mutex, pois se assume que quem a chama já está dentro da região crítica.
 * @return Ponteiro para o estado interno da aplicação.
 */
app_state_t* state_manager_get_state_pointer (void);

/**
 * @brief Altera as configurações de balanço de gama dos LEDs.
 * @param gamma_correction Ponteiro para uma struct local com os fatores de correção do LEDs.
 */
void state_manager_set_gamma_config (gamma_correction_t* gamma_correction);

#endif