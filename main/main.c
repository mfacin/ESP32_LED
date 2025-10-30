#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "driver/ledc.h"
#include "esp_err.h"

#define RED_GPIO 	(25)
#define GREEN_GPIO 	(26)
#define BLUE_GPIO 	(27)

#define RED_CHANNEL		LEDC_CHANNEL_0
#define GREEN_CHANNEL	LEDC_CHANNEL_1
#define BLUE_CHANNEL	LEDC_CHANNEL_2

#define SPEED_MODE	LEDC_LOW_SPEED_MODE
#define TIMER		LEDC_TIMER_0
#define DUTY_RES	LEDC_TIMER_8_BIT
#define FREQ_HZ		(5000)

#define DELAY_MS	(20)

#define TRUE		(1)
#define FALSE		(0)

void configure_ledc_channel (ledc_channel_t channel, int gpio_num)
{
	ledc_channel_config_t ledc_channel = {
		.speed_mode		= SPEED_MODE,
		.channel		= channel,
		.timer_sel		= TIMER,
		.intr_type		= LEDC_INTR_DISABLE,
		.gpio_num		= gpio_num,
		.duty			= 0,
		.hpoint			= 0
	};

	ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void update_duty (ledc_channel_t channel, int duty) {
	ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, channel, duty));
	ESP_ERROR_CHECK(ledc_update_duty(SPEED_MODE, channel));
}

void app_main (void)
{
	ledc_timer_config_t ledc_timer = {
		.speed_mode			= SPEED_MODE,
		.timer_num			= TIMER,
		.duty_resolution	= DUTY_RES,
		.freq_hz			= FREQ_HZ,
		.clk_cfg			= LEDC_AUTO_CLK
	};

	ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

	configure_ledc_channel(RED_CHANNEL, RED_GPIO);
	configure_ledc_channel(GREEN_CHANNEL, GREEN_GPIO);
	configure_ledc_channel(BLUE_CHANNEL, BLUE_GPIO);
	
	int red_duty_value = 254;
	int green_duty_value = 0;
	int blue_duty_value = 0;

	while (TRUE) {
		update_duty(RED_CHANNEL, red_duty_value);
		vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
		
		for (; green_duty_value < 255; green_duty_value ++) {
			update_duty(GREEN_CHANNEL, green_duty_value);
			vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
		}
		
		for (; red_duty_value >= 0; red_duty_value --) {
			update_duty(RED_CHANNEL, red_duty_value);
			vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
		}

		for (; blue_duty_value < 255; blue_duty_value ++) {
			update_duty(BLUE_CHANNEL, blue_duty_value);
			vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
		}

		for (; green_duty_value >= 0; green_duty_value --) {
			update_duty(GREEN_CHANNEL, green_duty_value);
			vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
		}

		for (; red_duty_value < 255; red_duty_value ++) {
			update_duty(RED_CHANNEL, red_duty_value);
			vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
		}

		for (; blue_duty_value >= 0; blue_duty_value --) {
			update_duty(BLUE_CHANNEL, blue_duty_value);
			vTaskDelay(DELAY_MS / portTICK_PERIOD_MS);
		}		
	}
}