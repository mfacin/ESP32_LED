#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include "colors.h"

/**
 * @brief Gera um número float pseudo-aleatório.
 * @param min Valor mínimo do intervalo desejado.
 * @param max Valor máximo do intervalo desejado.
 * @return Um número float pseudo-aleatório dentro do intervalo [min, max].
 */
float rand_float(float min, float max);

/**
 * @brief Gera um número inteiro pseudo-aleatório.
 * @param min Valor mínimo do intervalo desejado.
 * @param max Valor máximo do intervalo desejado.
 * @return Um número inteiro pseudo-aleatório dentro do intervalo [min, max].
 */
int rand_int(int min, int max);

/**
 * @brief Realiza a interpolação da velocidade configurada e máxima, para calcular o delay, em ms, necessário para mudança de cores.
 * @param speed Configuração de velocidade do efeito (0-100%).
 * @param max_speed Velocidade máxima permitida.
 * @param min_speed Velocidade mínima permitida.
 * @return O delay, em ms, referente à velocidade configurada, em relação com o máximo e mínimo.
 */
int interpolate_speed_to_delay (unsigned char speed, int max_speed, int min_speed);

/**
 * @brief Corrige o gama do LED para que as cores fiquem fieis ao definido.
 * @note Em quase todos os LEDs RGB, o LED verde é fisicamente mais eficiente e brilhante do que o LED vermelho, 
 * além de que o olho humano é naturalmente mais sensível aos tons de verde, tornado uma correção de gama necessária para 
 * correta percepção de cor.
 * @param color A cor definida, a ser corrigida.
 * @param red_factor Fator de correção de gama do LED vermelho.
 * @param green_factor Fator de correção de gama do LED verde.
 * @param blue_factor Fator de correção de gama do LED azul.
 * @return A cor RGB com seus respectivos valores corrigidos de acordo com os fatores providos.
 */
rgb_color_t correct_led_gamma (rgb_color_t color, float red_factor, float green_factor, float blue_factor);

#endif