#ifndef COLORS_H
#define COLORS_H

typedef struct {
	unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_color_t;

typedef struct {
	float h;
	float s;
	float v;
} hsv_color_t;

extern const rgb_color_t RED_RGB;
extern const rgb_color_t GREEN_RGB;
extern const rgb_color_t BLUE_RGB;
extern const rgb_color_t YELLOW_RGB;
extern const rgb_color_t CYAN_RGB;
extern const rgb_color_t VIOLET_RGB;
extern const rgb_color_t BLACK_RGB;
extern const rgb_color_t WHITE_RGB;

#endif