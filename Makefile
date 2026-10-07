CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS = -lclang
SRC = src/main.c src/matrix/matrix.c src/scanner/tree_scanner.c src/exporter/json_exporter.c src/ast/ast_traversal.c
TARGET = codemesh_engine

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)
