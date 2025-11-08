#ifndef COLORS_H
#define COLORS_H

/**
 * @brief Cor no formato RGB (8-bit por canal).
 */
typedef struct {
	unsigned char r; /**< Componente Vermelho (0-255). */
    unsigned char g; /**< Componente Verde (0-255). */
    unsigned char b; /**< Componente Azul (0-255). */
} rgb_color_t;

/**
 * @brief Cor no formato HSV (8-bit por canal).
 */
typedef struct {
	float h; /**< Componente Hue (0-360). */
	float s; /**< Componente Saturation (0-1). */
	float v; /**< Componente Value (0-1). */
} hsv_color_t;

/// @brief Vermelho Puro.
extern const hsv_color_t RED_HSV;
/// @brief Amarelo.
extern const hsv_color_t YELLOW_HSV;
/// @brief Ciano.
extern const hsv_color_t CYAN_HSV;
/// @brief Verde Puro.
extern const hsv_color_t GREEN_HSV;
/// @brief Violeta.
extern const hsv_color_t VIOLET_HSV;
/// @brief Azul Puro.
extern const hsv_color_t BLUE_HSV;
/// @brief Branco.
extern const hsv_color_t WHITE_HSV;
/// @brief Preto.
extern const hsv_color_t BLACK_HSV;

#endif