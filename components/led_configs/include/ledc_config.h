#ifndef LEDC_CONFIG_H
#define LEDC_CONFIG_H

#include "driver/gpio.h"
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

void configure_ledc_channel (ledc_channel_t channel, int gpio_num);

void update_duty (ledc_channel_t channel, int duty);

void configure_ledc ();

#endif