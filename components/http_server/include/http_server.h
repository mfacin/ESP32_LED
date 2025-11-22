#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "esp_http_server.h"

/**
 * @brief Inicializa o servidor HTTP para recebimento dos parâmetros da aplicação.
 * @return A instância do servidor inicializado, se houve sucesso. NULL, se houve erro.
 */
httpd_handle_t http_server_start (void);

#endif