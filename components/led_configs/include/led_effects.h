#ifndef LED_EFFECTS_H
#define LED_EFFECTS_H

#include <stdbool.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define DELAY_FADE		(10)	// delay, em ms, para mudança de cada cor no fade
#define DELAY_STROBE	(500)	// delay, em ms, para mudança de cores no strobe

void stationary_color (const rgb_color_t *color);

void effect_fade();
void effect_strobe ();
void effect_breath();

#endif