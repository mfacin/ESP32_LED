#include <stdlib.h>
#include <math.h>

float rand_float(float min, float max) {
    float r = ((float) rand() / (float) RAND_MAX) * (max - min) + min;
	return trunc(r * 100.0) / 100.0;
}

int rand_int(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}