#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

#include "hsv_to_rgb.h"
#include "ledc_config.h"
#include "led_effects.h"
#include "utils.h"
#include "colors.h"

#define COLOR_TIME	(2500)

void app_main (void) {
	srand(time(NULL));
	configure_ledc();

    int option = 0;
	while (true) {
		printf("Opção {%d}: ", option);

		switch (option) {
		case 0:
			printf("Efeito - Fade (Roda de cores)\n");
			effect_fade();
			effect_fade();
			break;
		case 1:
			printf("Efeito - Strobe\n");
			effect_strobe();
			effect_strobe();
			break;
		case 2:
			printf("Efeito - Breath\n");
			effect_breath();
			effect_breath();
			break;
		case 3:
			printf("Demonstração de cores:\n");

			printf("Preto:   RGB(%d, %d, %d)\n", BLACK_RGB.r, BLACK_RGB.g, BLACK_RGB.b);
			stationary_color(&BLACK_RGB);
            vTaskDelay(COLOR_TIME / portTICK_PERIOD_MS);

			printf("Vermelho:   RGB(%d, %d, %d)\n", RED_RGB.r, RED_RGB.g, RED_RGB.b);
			stationary_color(&RED_RGB);
            vTaskDelay(COLOR_TIME / portTICK_PERIOD_MS);

			printf("Amarelo:    RGB(%d, %d, %d)\n", YELLOW_RGB.r, YELLOW_RGB.g, YELLOW_RGB.b);
			stationary_color(&YELLOW_RGB);
            vTaskDelay(COLOR_TIME / portTICK_PERIOD_MS);

			printf("Verde:      RGB(%d, %d, %d)\n", GREEN_RGB.r, GREEN_RGB.g, GREEN_RGB.b);
			stationary_color(&GREEN_RGB);
            vTaskDelay(COLOR_TIME / portTICK_PERIOD_MS);

			printf("Ciano:      RGB(%d, %d, %d)\n", CYAN_RGB.r, CYAN_RGB.g, CYAN_RGB.b);
			stationary_color(&CYAN_RGB);
            vTaskDelay(COLOR_TIME / portTICK_PERIOD_MS);

			printf("Azul:       RGB(%d, %d, %d)\n", BLUE_RGB.r, BLUE_RGB.g, BLUE_RGB.b);
			stationary_color(&BLUE_RGB);
            vTaskDelay(COLOR_TIME / portTICK_PERIOD_MS);

			printf("Violeta:    RGB(%d, %d, %d)\n", VIOLET_RGB.r, VIOLET_RGB.g, VIOLET_RGB.b);
			stationary_color(&VIOLET_RGB);
            vTaskDelay(COLOR_TIME / portTICK_PERIOD_MS);

			printf("Branco:     RGB(%d, %d, %d)\n", WHITE_RGB.r, WHITE_RGB.g, WHITE_RGB.b);
			stationary_color(&WHITE_RGB);
            vTaskDelay(COLOR_TIME / portTICK_PERIOD_MS);
            break;
		default:
			hsv_color_t hsv_color = normalize_hsv(rand_float(0, 100), rand_float(0, 100), rand_float(0, 100));
			rgb_color_t rgb_color = hsv_to_rgb(hsv_color.h, hsv_color.s, hsv_color.v);
			stationary_color(&rgb_color);

			printf("Cor: HSV(%.2f, %.2f, %.2f) -> RGB(%d, %d, %d)\n", hsv_color.h, hsv_color.s, hsv_color.v, rgb_color.r, rgb_color.g, rgb_color.b);
			vTaskDelay(COLOR_TIME / portTICK_PERIOD_MS);
			break;
		}

        option = (option + 1) % 10;
	}
}