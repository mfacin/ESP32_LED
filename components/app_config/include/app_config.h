#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/// @brief Quantidade de passos que o efeito breath dará em uma direção.
#define BREATH_EFFECT_STEPS	(100.0)

/// @brief Delay para cor estacionária, apenas para controle no update().
#define STATIONARY_DELAY	(1000)

/// @brief Delay mínimo, em ms, do efeito de fade. Em suma, a velocidade máxima do efeito.
#define MIN_FADE_DELAY		(10)
/// @brief Delay mínimo, em ms, do efeito de strobe. Em suma, a velocidade máxima do efeito.
#define MIN_STROBE_DELAY	(100)
/// @brief Delay mínimo, em ms, do efeito de breath. Em suma, a velocidade máxima do efeito.
#define MIN_BREATH_DELAY	(10)

/// @brief Delay máximo, em ms, do efeito de fade. Em suma, a velocidade mínima do efeito.
#define MAX_FADE_DELAY		(100)
/// @brief Delay máximo, em ms, do efeito de strobe. Em suma, a velocidade mínima do efeito.
#define MAX_STROBE_DELAY	(5000)
/// @brief Delay máximo, em ms, do efeito de breath. Em suma, a velocidade mínima do efeito.
#define MAX_BREATH_DELAY	(100)

/// @brief Fator de correção do gama vermelho.
#define RED_GAMMA_CORRECTION	(1.00)
/// @brief Fator de correção do gama verde.
#define GREEN_GAMMA_CORRECTION	(0.45)
/// @brief Fator de correção do gama azul.
#define BLUE_GAMMA_CORRECTION	(0.31)

/// @brief Configuração de GPIO do LED vermelho.
#define RED_GPIO	(25)
/// @brief Configuração de GPIO do LED verde.
#define GREEN_GPIO	(26)
/// @brief Configuração de GPIO do LED azul.
#define BLUE_GPIO	(27)

/// @brief Configuração de canal do LEDC do LED vermelho.
#define RED_CHANNEL		LEDC_CHANNEL_0
/// @brief Configuração de canal do LEDC do LED verde.
#define GREEN_CHANNEL	LEDC_CHANNEL_1
/// @brief Configuração de canal do LEDC do LED azul.
#define BLUE_CHANNEL	LEDC_CHANNEL_2

/// @brief Configuração de velocidade do LEDC.
#define SPEED_MODE	LEDC_LOW_SPEED_MODE
/// @brief Configuração de timer do LEDC.
#define TIMER		LEDC_TIMER_0
/// @brief Configuração de resolução de duty do LEDC.
#define DUTY_RES	LEDC_TIMER_8_BIT
/// @brief Configuração de frequeência do LEDC.
#define FREQ_HZ		(5000)

/// @brief Namespace para as configurações de gama dos LEDs na NVS.
#define NVS_NAMESPACE	"led_config"
/// @brief Chave para o fator de correção do gama vermelho.
#define NVS_KEY_GAMMA_R	"gamma_r"
/// @brief Chave para o fator de correção do gama verde.
#define NVS_KEY_GAMMA_G	"gamma_g"
/// @brief Chave para o fator de correção do gama azul.
#define NVS_KEY_GAMMA_B	"gamma_b"
/// @brief Chave para o efeito configurado.
#define NVS_KEY_EFFECT	"effect"
/// @brief Chave para a velocidade do efeito configurado.
#define NVS_KEY_SPEED	"speed"
/// @brief Chave para o valor de hue (matiz).
#define NVS_KEY_HUE			"hue"
/// @brief Chave para o valor de saturação.
#define NVS_KEY_SATURATION	"saturation"
/// @brief Chave para o valor de brilho.
#define NVS_KEY_BRIGHTNESS	"brightness"
/// @brief Chave para o status de ON/OFF.
#define NVS_KEY_ONOFF	"onoff"

/// @brief SSID da rede Wi-Fi a ser conectada.
/// @note Verifique que o arquivo /main/Kconfig.projbuild contém as definições de configuração das opções WIFI_SSID e WIFI_PASSWD.
/// @note Essas configurações podem ser definidas nas chaves CONFIG_WIFI_SSID e CONFIG_WIFI_PASSWD no arquivo /sdkconfig
#define WIFI_SSID		CONFIG_WIFI_SSID
/// @note Verifique que o arquivo /main/Kconfig.projbuild contém as definições de configuração das opções WIFI_SSID e WIFI_PASSWD.
/// @note Essas configurações podem ser definidas nas chaves CONFIG_WIFI_SSID e CONFIG_WIFI_PASSWD no arquivo /sdkconfig
/// @brief Senha da rede Wi-Fi a ser conectada.
#define WIFI_PASS		CONFIG_WIFI_PASSWD
/// @brief Número máximo de tentativas de conexão à rede Wi-Fi.
#define WIFI_MAX_RETRY	10

#endif