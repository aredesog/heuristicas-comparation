# Makefile - Projeto de Heurísticas para Problema do Caixeiro Viajante
# Compilador e flags
CC := gcc
CFLAGS := -Wall -Wextra -O2 -std=c99
LDFLAGS := -lm

# Diretórios
SRC_DIR := tp-4/src
INC_DIR := tp-4/include
OBJ_DIR := build
BIN_DIR := bin

# Includes
INCLUDES := -I$(INC_DIR) -I.

# Arquivos fonte
# Fontes da raiz
ROOT_SRCS := base.c gera.c heuristicas.c

# Fontes de tp-4/src
TP4_SRCS := $(addprefix $(SRC_DIR)/, genetic.c main.c opt.c tsp.c vizinho.c)

# Todos os fontes
ALL_SRCS := $(ROOT_SRCS) $(TP4_SRCS)

# Arquivos objeto
ROOT_OBJS := $(addprefix $(OBJ_DIR)/, $(ROOT_SRCS:.c=.o))
TP4_OBJS := $(addprefix $(OBJ_DIR)/, $(TP4_SRCS:.c=.o))
ALL_OBJS := $(ROOT_OBJS) $(TP4_OBJS)

# Executável
TARGET := $(BIN_DIR)/tsp

# Regras padrão
.PHONY: all clean run help

# Alvo padrão
all: $(TARGET)

# Criar o executável
$(TARGET): $(ALL_OBJS) | $(BIN_DIR)
	@echo "Ligando executável..."
	$(CC) $(ALL_OBJS) $(LDFLAGS) -o $@
	@echo "Executável criado: $@"

# Compilar objetos da raiz
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compilar objetos de tp-4/src
$(OBJ_DIR)/tp-4/src/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/tp-4/src
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Criar diretórios se não existirem
$(OBJ_DIR) $(BIN_DIR) $(OBJ_DIR)/tp-4/src:
	mkdir -p $@

# Limpar arquivos compilados
clean:
	@echo "Limpando arquivos compilados..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Limpeza concluída."

# Executar o programa
run: $(TARGET)
	@echo "Executando $(TARGET)..."
	./$(TARGET)

# Mostrar ajuda
help:
	@echo "Alvos disponíveis:"
	@echo "  make          - Compilar o projeto"
	@echo "  make clean    - Remover arquivos compilados"
	@echo "  make run      - Compilar e executar"
	@echo "  make help     - Mostrar esta mensagem"
