#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

// Enum para os diferentes tipos de mensagem entre cliente e servidor
typedef enum {
    MSG_TYPE_BET,       // Cliente envia uma aposta
    MSG_TYPE_CASHOUT,   // Cliente solicita o saque
    MSG_TYPE_UPDATE,    // Servidor envia atualização do multiplicador
    MSG_TYPE_RESULT,    // Servidor envia o resultado final da rodada
    MSG_TYPE_INFO,      // Servidor envia mensagens de status (Ex: "Apostas abertas")
    MSG_TYPE_ERROR      // Servidor envia uma mensagem de erro
} msg_type_t;

// Enum para as fases do jogo
typedef enum {
    GAME_STATE_BETTING, // Fase de apostas
    GAME_STATE_FLYING,  // O avião está voando
    GAME_STATE_EXPLODED // O avião explodiu
} game_state_t;

// Estrutura para as mensagens trocadas
typedef struct {
    msg_type_t type;
    char message[256];
    double value; // Usado para valores de aposta, multiplicadores, etc.
} aviator_msg;

// Função utilitária para encerrar o programa em caso de erro fatal
static inline void die(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

#endif // COMMON_H