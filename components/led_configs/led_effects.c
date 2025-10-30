#include <stdbool.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ledc_config.h"
#include "colors.h"
#include "led_effects.h"

void stationary_color (const rgb_color_t *color) {
	update_duty(RED_CHANNEL, color->r);
	update_duty(GREEN_CHANNEL, color->g);
	update_duty(BLUE_CHANNEL, color->b);
}

void effect_fade () {
	int red_duty_value = 254;
	int green_duty_value = 0;
	int blue_duty_value = 0;

	// reseta para o vermelho
	stationary_color(&RED_RGB);
	vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	
	// aumenta o verde, para chegar no amarelo
	for (; green_duty_value < 255; green_duty_value ++) {
		update_duty(GREEN_CHANNEL, green_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}
	
	// diminui o vermelho, para chegar no verde
	for (; red_duty_value >= 0; red_duty_value --) {
		update_duty(RED_CHANNEL, red_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	// aumenta o azul, para chegar no 
	for (; blue_duty_value < 255; blue_duty_value ++) {
		update_duty(BLUE_CHANNEL, blue_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; green_duty_value >= 0; green_duty_value --) {
		update_duty(GREEN_CHANNEL, green_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; red_duty_value < 255; red_duty_value ++) {
		update_duty(RED_CHANNEL, red_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; blue_duty_value >= 0; blue_duty_value --) {
		update_duty(BLUE_CHANNEL, blue_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}
}

void effect_strobe () {
	stationary_color(&RED_RGB);
	vTaskDelay(DELAY_STROBE / portTICK_PERIOD_MS);
	stationary_color(&YELLOW_RGB);
	vTaskDelay(DELAY_STROBE / portTICK_PERIOD_MS);
	stationary_color(&GREEN_RGB);
	vTaskDelay(DELAY_STROBE / portTICK_PERIOD_MS);
	stationary_color(&CYAN_RGB);
	vTaskDelay(DELAY_STROBE / portTICK_PERIOD_MS);
	stationary_color(&BLUE_RGB);
	vTaskDelay(DELAY_STROBE / portTICK_PERIOD_MS);
	stationary_color(&VIOLET_RGB);
}

void effect_breath() {
	int red_duty_value = 0;
	int yellow_duty_value = 0;
	int green_duty_value = 0;
	int cyan_duty_value = 0;
	int blue_duty_value = 0;
	int violet_duty_value = 0;
	int white_duty_value = 0;

	stationary_color(&BLACK_RGB);

	// vermelho
	for (; red_duty_value < 255; red_duty_value ++) {
		update_duty(RED_CHANNEL, red_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; red_duty_value >= 0; red_duty_value --) {
		update_duty(RED_CHANNEL, red_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	// amarelo
	for (; yellow_duty_value < 255; yellow_duty_value ++) {
		update_duty(RED_CHANNEL, yellow_duty_value);
		update_duty(GREEN_CHANNEL, yellow_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; yellow_duty_value >= 0; yellow_duty_value --) {
		update_duty(RED_CHANNEL, yellow_duty_value);
		update_duty(GREEN_CHANNEL, yellow_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	// verde
	for (; green_duty_value < 255; green_duty_value ++) {
		update_duty(GREEN_CHANNEL, green_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; green_duty_value >= 0; green_duty_value --) {
		update_duty(GREEN_CHANNEL, green_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	// ciano
	for (; cyan_duty_value < 255; cyan_duty_value ++) {
		update_duty(GREEN_CHANNEL, cyan_duty_value);
		update_duty(BLUE_CHANNEL, cyan_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; cyan_duty_value >= 0; cyan_duty_value --) {
		update_duty(GREEN_CHANNEL, cyan_duty_value);
		update_duty(BLUE_CHANNEL, cyan_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	// azul
	for (; blue_duty_value < 255; blue_duty_value ++) {
		update_duty(BLUE_CHANNEL, blue_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; blue_duty_value >= 0; blue_duty_value --) {
		update_duty(BLUE_CHANNEL, blue_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	// violeta
	for (; violet_duty_value < 255; violet_duty_value ++) {
		update_duty(BLUE_CHANNEL, violet_duty_value);
		update_duty(RED_CHANNEL, violet_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; violet_duty_value >= 0; violet_duty_value --) {
		update_duty(BLUE_CHANNEL, violet_duty_value);
		update_duty(RED_CHANNEL, violet_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	// branco
	for (; white_duty_value < 255; white_duty_value ++) {
		update_duty(RED_CHANNEL, white_duty_value);
		update_duty(GREEN_CHANNEL, white_duty_value);
		update_duty(BLUE_CHANNEL, white_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}

	for (; white_duty_value >= 0; white_duty_value --) {
		update_duty(RED_CHANNEL, white_duty_value);
		update_duty(GREEN_CHANNEL, white_duty_value);
		update_duty(BLUE_CHANNEL, white_duty_value);
		vTaskDelay(DELAY_FADE / portTICK_PERIOD_MS);
	}
	
}