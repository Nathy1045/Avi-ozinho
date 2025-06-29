```
/*
 * ===================================================================================
 * Código Comum (common.c)
 *
 * Propósito:
 * Criar as funções auxiliares definidas em "common.h".
 * Este arquivo é compilado e associado ao executável do servidor e
 * ao do cliente, possibilitando o compartilhamento de funcionalidades.
 * ===================================================================================
 */

#include "common.h" // Importa as definições do protocolo e os protótipos de funções.

/**
 * @brief Apresenta uma mensagem de erro e finaliza o programa.
 *
 * Esta função atua como uma camada para concentrar o gerenciamento de erros críticos.
 * Ela chama a função `perror` para exibir a mensagem de erro que foi fornecida,
 * seguida de uma explicação sobre o último erro do sistema (provido pela variável `errno`).
 * Isso é muito útil para diagnosticar questões relacionadas a chamadas de sistema,
 * como falhas em `socket()`, `bind()`, `connect()`, e outros.
 *
 * @param message A mensagem descritiva de erro que deve ser apresentada.
 */
void die(const char *message) {
    // perror exibe a string 'message', seguida de dois-pontos, um espaço,
    // e a mensagem de erro que corresponde ao valor atual de `errno`.
    perror(message);

    // Termina o programa de forma imediata, retornando um código de erro
    // ao sistema operacional.
    exit(EXIT_FAILURE);
}

/*
 * ===================================================================================
 * EXPLICAÇÃO :
 *
 * Este arquivo é um excelente lugar para incluir funções adicionais que sejam
 * úteis tanto para o cliente quanto para o servidor. Por exemplo:
 *
 * - Funções para transmitir e receber a 'struct aviator_msg' de forma garantida,
 * assegurando que todos os bytes sejam enviados (tratando gravações
 * e leituras parciais em sockets TCP).
 *
 * - Funções para registro que formatam e apresentam mensagens no terminal
 * de forma uniforme.
 *
 * Organizar o código compartilhado aqui ajuda a inibir redundâncias e torna
 * o projeto mais simples de gerenciar.
 * ===================================================================================
 */
```