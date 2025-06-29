# Makefile para o projeto Aviador (Trabalho Prático 2)
#
# Este documento controla o processo de compilação tanto do cliente quanto do servidor,
# organizando os arquivos objeto e os executáveis finais
# nas pastas 'obj' e 'bin', respectivamente, conforme
# definido nas diretrizes do projeto.

# --- Variáveis de Compilação ---

# Compilador C que será utilizado (padrão: gcc)
CC = gcc

# Opções de compilação:
# -g: Insere informações de depuração (debug)
# -Wall: Habilita a maioria dos avisos (warnings)
# -Wextra: Habilita avisos adicionais
# -Iinclude: Informa ao compilador para buscar arquivos de cabeçalho na pasta 'include'
# -pthread: Necessário para a compilação com a biblioteca de pthreads (multithreading)
CFLAGS = -g -Wall -Wextra -Iinclude -pthread

# --- Estrutura de Diretórios ---

# Pasta onde os arquivos de código fonte (.c) são armazenados
SRC_DIR = src

# Pasta para guardar os arquivos objeto (.o) criados durante a compilação
OBJ_DIR = obj

# Pasta onde os executáveis finais (client e server) serão alocados
BIN_DIR = bin

# --- Arquivos do Projeto ---

# Conjunto de todos os arquivos de código fonte
# O comando 'wildcard' localiza todos os arquivos .c no diretório 'src'
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Transforma a lista de arquivos .c para seus equivalentes .o no diretório 'obj'
# Por exemplo: src/server.c se torna obj/server.o
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Determina os nomes dos executáveis finais
SERVER_EXE = $(BIN_DIR)/server
CLIENT_EXE = $(BIN_DIR)/client

# --- Regras de Compilação (Targets) ---

# A regra 'all' é a principal, que é executada quando 'make' é chamado sem parâmetros.
# Garante a existência dos diretórios e compila ambos os executáveis.
#
all: directories $(SERVER_EXE) $(CLIENT_EXE)

# Regra para gerar o executável do servidor.
# Depende do arquivo de objeto 'server.o'.
# Inclua outros arquivos .o se o servidor for modular (ex: $(OBJ_DIR)/utils.o)
$(SERVER_EXE): $(OBJ_DIR)/server.o
	@echo "Construindo os arquivos objeto para criar o executável do servidor..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Servidor compilado com sucesso em $(SERVER_EXE)"

# Regra para gerar o executável do cliente.
# Depende do arquivo de objeto 'client.o'.
# Inclua outros .o se necessário (ex: $(OBJ_DIR)/utils.o)
$(CLIENT_EXE): $(OBJ_DIR)/client.o
	@echo "Construindo os arquivos objeto para criar o executável do cliente..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Cliente compilado com sucesso em $(CLIENT_EXE)"

# Regra padrão para compilar qualquer arquivo .c da pasta 'src'
# em um arquivo .o na pasta 'obj'.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compilando $&lt; -&gt; $@"
	$(CC) $(CFLAGS) -c $&lt; -o $@

# Regra para criar os diretórios 'obj' e 'bin' caso ainda não existam.
# O '@' oculta a apresentação do comando na tela.
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# --- Regras de Limpeza ---

# A regra 'clean' remove os diretórios e arquivos gerados durante a compilação.
# Fundamental para assegurara recompilação limpa do início.
clean: 
@echo "Deleting generated files..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Cleanup finished."

# Declaration that 'all' and 'clean' are not file names.
# It is a good practice to prevent conflicts with files that may have these names.
.PHONY: all clean directories