#include <stdio.h>
#include <math.h>
#include "colors.h"

/*
 * Normalização dos dados recebidos pelo SmartThings
 * H: 0.0 - 100.0 (porcentagem) -> 0.0 - 360.0 (graus)
 * S: 0.0 - 100.0 (porcentagem) -> 0.0 - 1.0 (porcentavem)
 * V: 0.0 - 100.0 (porcentagem) -> 0.0 - 1.0 (porcentavem)
 */
hsv_color_t normalize_hsv(float h, float s, float v) {
	hsv_color_t normalized_hsv = {
		.h = (trunc(h * 100) / 10000) * 360,
		.s = trunc(s) / 100.0,
		.v = trunc(s) / 100.0
	};

	return normalized_hsv;
}

/*
 * Conversão de HSV para RGB
 * H: 0.0 - 360.0 (graus)
 * S: 0.0 - 1.0   (porcentagem)
 * V: 0.0 - 1.0   (porcentagem)
 */
rgb_color_t hsv_to_rgb(float h, float s, float v) {
	/* 
	 * https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
	 *
	 * 1. H ∈ [0°, 360°), S ∈ [0, 1], V ∈ [0, 1]
	 * 2. C = V x S
	 * 3. H' = H / 60º
	 * 4. X = C x (1 - |H' mod 2 - 1|)
	 * 
	 * 5. (R1, G1, B1) = {
	 *     (C, X, 0)	if 0 <= H' < 1
	 *     (X, C, 0)	if 1 <= H' < 2
	 *     (0, C, X)	if 2 <= H' < 3
	 *     (0, X, C)	if 3 <= H' < 4
	 *     (X, 0, C)	if 4 <= H' < 5
	 *     (C, 0, X)	if 5 <= H' < 6
	 * }
	 * 
	 * 6. m = V - C
	 * 7. (R, G, B) = (R1 + m, G1 + m, B1 + m)
	 */

	// 1. Validando se os dados estão dentro dos limites
	if (h > 360) h = fmod(h, 360.0); 
    if (h < 0) h += 360;
    if (s > 1) s = 1; 
    if (s < 0) s = 0;
    if (v > 1) v = 1;
    if (v < 0) v = 0;

	float c = v * s;		// 2. Calculando o Chroma
	float h1 = h / 60.0;	// 3. Calculando o H'
	float x = c * (1.0 - fabs(fmod(h1, 2.0) - 1.0));	// 4. Calculando o intermediário X
	
	// 5. Encontrando os valores intermediários
	float r1 = 0.0, g1 = 0.0, b1 = 0.0;
	if (0 <= h1 && h1 < 1) {
		r1 = c; g1 = x; b1 = 0;
	} else if (1 <= h1 && h1 < 2) {
		r1 = x; g1 = c; b1 = 0;
	} else if (2 <= h1 && h1 < 3) {
		r1 = 0; g1 = c; b1 = x;
	} else if (3 <= h1 && h1 < 4) {
		r1 = 0; g1 = x; b1 = c;
	} else if (4 <= h1 && h1 < 5) {
		r1 = x; g1 = 0; b1 = c;
	} else if (5 <= h1 && h1 < 6) {
		r1 = c; g1 = 0; b1 = x;
	}

	// 6. Calculando o m
	float m = v - c;

	// Após a implementação do algoritmo, foi possível notar erros quando a saturação é 0
	if (s == 0.0) {
		r1 = v; g1 = v; b1 = v; m = 0;
	}

	// 7. Calculando os valores finais
	// A múltiplicação por 255 é necessária, pois o cálculo original considera R, G, e B ∈ [0, 1]
	// A soma do 0,5 é necessária para evitar truncamento e melhororar o arredondamento (evita que 254,999 vire 254)
	rgb_color_t rgb = {
		.r = (unsigned char) ((r1 + m) * 255.0 + 0.5),
		.g = (unsigned char) ((g1 + m) * 255.0 + 0.5),
		.b = (unsigned char) ((b1 + m) * 255.0 + 0.5)
	};

    return rgb;
}