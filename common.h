#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>

// Define a capacidade máxima de jogadores no servidor.
#define MAX_PLAYERS 10

/**
 * @brief Estrutura da mensagem trocada entre o servidor e os jogadores.
 *
 * Esta estrutura pode ser expandida no futuro para acomodar novas
 * funcionalidades do jogo.
 */
struct aviator_msg {
    char type[16];      // Tipo da mensagem (ex: "bet", "cashout").
    int bet_amount;     // Quantia da aposta.
};

/**
 * @brief Agrupa as informações de cada cliente conectado ao servidor.
 *
 * Cada conexão de jogador é representada por uma instância desta estrutura,
 * que contém os dados necessários para seu gerenciamento.
 */
typedef struct {
    int socket_fd;      // Descritor do socket para a comunicação.
    int player_id;      // Identificador único atribuído ao jogador.
    pthread_t thread_id; // ID da thread que gerencia a conexão deste cliente.
} client_info_t;


// Mutex para sincronizar o acesso ao array de clientes e evitar race conditions.
extern pthread_mutex_t clients_mutex;

// Array global para armazenar os dados de todos os clientes conectados.
extern client_info_t clients[MAX_PLAYERS];

// Variável para controlar o próximo ID a ser atribuído a um novo jogador.
extern int next_player_id;


// ---- Declarações das Funções do Servidor ----

/**
 * @brief Inicializa o estado do servidor, incluindo variáveis globais e mutex.
 */
void initialize_server_state(void);

/**
 * @brief Função alvo para a thread de cada cliente. Gerencia a comunicação.
 * @param arg Um ponteiro para a estrutura client_info_t do cliente.
 */
void *handle_client(void *arg);

/**
 * @brief Função alvo para a thread principal do jogo. Controla a lógica da partida.
 * @param arg Argumentos para a thread do jogo (pode ser NULL).
 */
void *game_loop(void *arg);

#endif // COMMON_H