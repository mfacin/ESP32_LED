#include "hsv_to_rgb.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

float rand_float(float min, float max) {
    float r = ((float) rand() / (float) RAND_MAX) * (max - min) + min;
	return trunc(r * 100.0) / 100.0;
}

int main() {
	srand(time(NULL));

	for (int i = 0; i < 20; i++) {
		hsv_color_t hsv_color = normalize_hsv(rand_float(0, 100), rand_float(0, 100), rand_float(0, 100));

		rgb_color_t rgb_color = hsv_to_rgb(hsv_color.h, hsv_color.s, hsv_color.v);

		printf("%.2f, %.2f, %.2f, %d, %d, %d\n", hsv_color.h, hsv_color.s, hsv_color.v, rgb_color.r, rgb_color.g, rgb_color.b);
	}
}