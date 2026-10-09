CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -I/opt/homebrew/opt/llvm/include
LDFLAGS = -L/opt/homebrew/opt/llvm/lib -lclang

# Main Engine — now includes AST analysis + matrix detection
SRC = src/main.c \
      src/scanner/tree_scanner.c \
      src/exporter/json_exporter.c \
      src/matrix/matrix.c \
      src/matrix/matrix_detector.c \
      src/ast/ast_traversal.c
TARGET = codemesh_engine

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

run-test: $(TARGET)
	./$(TARGET) test

clean:
	rm -f $(TARGET) ast_detector ast_traversal ast *.o

.PHONY: all run run-test clean