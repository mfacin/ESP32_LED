#ifndef HSV_TO_RGB_H
#define HSV_TO_RGB_H

#include <math.h>
#include <stdio.h>
#include "colors.h"

/**
 * @brief Normaliza os dados recebidos pelo SmartThings.
 * 
 * @param h Hue (0.0-100.0 %).
 * @param s Saturation (0.0-100.0 %).
 * @param v Value (0.0-100.0 %).
 * 
 * @return Uma struct hsv_color_t com os valores HSV normalizados (0-360, 0-1, 0-1).
 */
hsv_color_t normalize_hsv(float h, float s, float v);

/**
 * @brief Converte uma cor no espaço HSV para RGB.
 * 
 * @param hsv_color Cor HSV para ser convertida.
 * 
 * @return Uma struct rgb_color_t com os valores RGB (0-255).
 */
rgb_color_t hsv_to_rgb(hsv_color_t hsv_color);

#endif