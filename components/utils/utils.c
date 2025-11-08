#include <stdlib.h>
#include <math.h>

#include "utils.h"

float rand_float (float min, float max) {
    float r = ((float) rand() / (float) RAND_MAX) * (max - min) + min;
	return trunc(r * 100.0) / 100.0;
}

int rand_int (int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

int interpolate_speed_to_delay (unsigned char speed, int max_speed, int min_speed) {
    // Normaliza a velocidade para float [0, 1]
    float s = (float) speed / 100.0;

    // Interpola lineramente a velocidade
    // s=0.0 (velocidade 0%)    -> 1.0 * max_speed + 0 = 100ms
    // s=1.0 (velocidade 100%)  -> 0 + 1.0 * min_speed = 1ms
    // s=0.5 (velocidade 50%)   -> (0.5 * max_speed) + (0.5 * min_speed) = 50.5ms
    float delay = ((1.0 - s) * max_speed) + (s * min_speed);

    return (int) delay;
}

rgb_color_t correct_led_gamma (rgb_color_t color, float red_factor, float green_factor, float blue_factor) {
	rgb_color_t final_color = {
		.r = (unsigned char) (color.r * red_factor),
		.g = (unsigned char) (color.g * green_factor),
		.b = (unsigned char) (color.b * blue_factor)
	};

	return final_color;
}