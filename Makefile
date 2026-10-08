CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -I/opt/homebrew/opt/llvm/include
LDFLAGS = -L/opt/homebrew/opt/llvm/lib -lclang

# Main Engine Target
SRC = src/main.c src/matrix/matrix.c src/scanner/tree_scanner.c src/exporter/json_exporter.c
TARGET = codemesh_engine

# AST Matrix Detector Standalone Target
AST_SRC = src/ast/ast_traversal.c src/matrix/matrix_detector.c src/matrix/matrix.c
AST_TARGET = ast_detector

all: $(TARGET) $(AST_TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

$(AST_TARGET): $(AST_SRC)
	$(CC) $(CFLAGS) $(AST_SRC) $(LDFLAGS) -o $(AST_TARGET)

run: $(TARGET)
	./$(TARGET)

run-ast: $(AST_TARGET)
	./$(AST_TARGET) test/test.c

clean:
	rm -f $(TARGET) $(AST_TARGET) *.o

.PHONY: all run run-ast clean