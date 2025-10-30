#ifndef HSV_TO_RGB_H
#define HSV_TO_RGB_H

#include <math.h>
#include <stdio.h>

typedef struct {
	unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_color_t;

typedef struct {
	float h;
	float s;
	float v;
} hsv_color_t;

/*
 * Normalização dos dados recebidos pelo SmartThings
 * H: 0.0 - 100.0 (porcentagem) -> 0.0 - 360.0 (graus)
 * S: 0.0 - 100.0 (porcentagem) -> 0.0 - 1.0 (porcentavem)
 * V: 0.0 - 100.0 (porcentagem) -> 0.0 - 1.0 (porcentavem)
 */
hsv_color_t normalize_hsv(float h, float s, float v);

/*
 * Conversão de HSV para RGB
 * H: 0.0 - 360.0 (graus)
 * S: 0.0 - 1.0   (porcentagem)
 * V: 0.0 - 1.0   (porcentagem)
 */
rgb_color_t hsv_to_rgb(float h, float s, float v);

#endif