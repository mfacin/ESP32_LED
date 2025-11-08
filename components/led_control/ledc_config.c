#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"

#include "ledc_config.h"

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

void configure_ledc () {
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
}