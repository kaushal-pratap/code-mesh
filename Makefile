
CC = gcc

# Common compiler flags
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

# LLVM / libclang configuration (Main Engine only)
LLVM_CFLAGS = -I/opt/homebrew/opt/llvm/include
LLVM_LDFLAGS = -L/opt/homebrew/opt/llvm/lib -lclang

# Main Engine — AST analysis + matrix detection
SRC = src/main.c \
      src/scanner/tree_scanner.c \
      src/exporter/json_exporter.c \
      src/matrix/matrix.c \
      src/matrix/matrix_detector.c \
      src/ast/ast_traversal.c

TARGET = codemesh_engine

# HTTP Server
SERVER_SRC = src/server/server_main.c \
             src/server/http_server.c \
             src/server/http_parser.c \
             src/server/http_response.c \
             src/server/router.c

SERVER_TARGET = code_mesh_server

# Default target
all: $(TARGET)

# Build Main Engine
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LLVM_CFLAGS) $(SRC) $(LLVM_LDFLAGS) -o $(TARGET)

# Build HTTP Server
server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER_TARGET)

# Run Main Engine
run: $(TARGET)
	./$(TARGET)

run-test: $(TARGET)
	./$(TARGET) test

# Run HTTP Server
run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

# Existing launcher
start:
	./run.sh

# Clean generated executables
clean:
	rm -f $(TARGET) $(SERVER_TARGET) ast_detector ast_traversal ast *.o

.PHONY: all server run run-test run-server start clean
