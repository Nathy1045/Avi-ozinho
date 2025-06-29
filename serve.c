/*
 * ===================================================================================
 * Arquivo: servidor.c
 *
 * Objetivo:
 * Desenvolver um servidor multithread para o jogo "Aviator". Este servidor
 * é encarregado de administrar as ligações dos jogadores, supervisionar as
 * rodadas do jogo, calcular as apostas e os pagamentos, além de manter o estado
 * geral do jogo de maneira síncrona e segura.
 * ===================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include "common.h"

#define MAX_PLAYERS 10

// --- Estruturas de Dados ---
typedef struct {
    int socket_fd;
    int player_id;
    pthread_t thread_id;
    double bet_amount;
    int has_bet;
    int has_cashed_out;
    double cashout_multiplier;
} client_info_t;

// --- Variáveis Globais (Estado do Servidor) ---
client_info_t clients[MAX_PLAYERS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
int next_player_id = 1;

game_state_t current_game_state;
double current_multiplier;
double explosion_multiplier;
pthread_mutex_t game_state_mutex = PTHREAD_MUTEX_INITIALIZER;


// --- Protótipos de Funções ---
void *handle_client(void *arg);
void *game_loop(void *arg);
void initialize_server_state();
void broadcast_message(aviator_msg *msg);

// ===================================================================================
// Função Principal (main)
// ===================================================================================
int main(int argc, char *argv[]) {
    // 1. Verificar os parâmetros da linha de comando
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <protocolo> <porta>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *protocol = argv[1];
    const char *port = argv[2];
    int family;

    if (strcmp(protocol, "v4") == 0) {
        family = AF_INET;
    } else if (strcmp(protocol, "v6") == 0) {
        family = AF_INET6;
    } else {
        fprintf(stderr, "Erro: Protocolo inválido. Utilize 'v4' ou 'v6'.\n");
        exit(EXIT_FAILURE);
    }

    // 2. Preparar o estado do servidor
    initialize_server_state();

    // 3. Definir o endereço do servidor utilizando getaddrinfo
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // Para bind

    int status = getaddrinfo(NULL, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    // 4. Criar o socket de escuta
    int listen_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (listen_fd < 0) {
        die("Erro ao criar o socket");
    }

    // Permitir a reutilização do endereço para evitar "Address already in use"
    int yes = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        die("Erro em setsockopt");
    }

    // 5. Associar (bind) o socket ao endereço e porta
    if (bind(listen_fd, res->ai_addr, res->ai_addrlen) < 0) {
        close(listen_fd);
        die("Erro ao fazer o bind");
    }

    freeaddrinfo(res); // Liberar a memória da estrutura de endereços

    // 6. Colocar o socket em modo de escuta
    if (listen(listen_fd, MAX_PLAYERS) < 0) {
        die("Erro no listen");
    }

    printf("Servidor Aviator iniciado. Aguardando conexões na porta %s...\n", port);

    // 7. Iniciar a thread principal do loop do jogo
    pthread_t game_thread_id;
    if (pthread_create(&game_thread_id, NULL, game_loop, NULL) != 0) {
        die("Erro ao criar a thread do jogo");
    }

    // 8. Loop principal para aceitar novas conexões
    while (1) {
        struct sockaddr_storage client_addr;
        socklen_t addr_size = sizeof(client_addr);
        int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_size);
        if (client_fd < 0) {
            perror("Error in accept");
            continue;
        }

        pthread_mutex_lock(&clients_mutex);

        int i;
        for (i = 0; i < MAX_PLAYERS; i++) {
            if (clients[i].socket_fd == -1) {
                clients[i].socket_fd = client_fd;
                clients[i].player_id = next_player_id++;
                
                // Criar uma thread para gerenciar este cliente
                if (pthread_create(&clients[i].thread_id, NULL, handle_client, &clients[i]) != 0) {
                    perror("Error creating client thread");
                    clients[i].socket_fd = -1; // Resetar slot
                }
                break;
            }
        }

        if (i == MAX_PLAYERS) {
            fprintf(stderr, "Server is full. Rejecting connection.\n");
            close(client_fd);
        }

        pthread_mutex_unlock(&clients_mutex);
    }

    close(listen_fd);
    return 0;
}

// --- Funções de Thread ---

void *handle_client(void *arg) {
    client_info_t *client = (client_info_t *)arg;
    int client_fd = client->socket_fd;
    int player_id = client->player_id;
    
    printf("Player %d connected.\n", player_id);

    struct aviator_msg msg;
    while (recv(client_fd, &msg, sizeof(msg), 0) > 0) {
        // Lógica para processar mensagens do cliente (aposta, cashout, etc.)
    }

    // Cliente desconectou
    printf("Player %d disconnected.\n", player_id);
    close(client_fd);
    
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (clients[i].player_id == player_id) {
            clients[i].socket_fd = -1; // Marcar slot como disponível
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    return NULL;
}

void *game_loop(void *arg) {
    while (1) {
        // Lógica do jogo (Fases de Aposta, Voo, Explosão)
        sleep(5); // Placeholder
    }
    return NULL;
}

// --- Funções Auxiliares ---

void initialize_server_state() {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_PLAYERS; i++) {
        clients[i].socket_fd = -1;
        clients[i].player_id = -1;
    }
    pthread_mutex_unlock(&clients_mutex);
}