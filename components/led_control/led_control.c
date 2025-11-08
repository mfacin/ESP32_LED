#include "led_control.h"

void set_color (rgb_color_t rgb_color) {
	update_duty(RED_CHANNEL, rgb_color.r);
	update_duty(GREEN_CHANNEL, rgb_color.g);
	update_duty(BLUE_CHANNEL, rgb_color.b);
}

void update_duty (ledc_channel_t channel, int duty) {
	ESP_ERROR_CHECK(ledc_set_duty(SPEED_MODE, channel, duty));
	ESP_ERROR_CHECK(ledc_update_duty(SPEED_MODE, channel));
}