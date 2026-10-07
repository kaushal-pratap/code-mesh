#ifndef TREE_SCANNER_H
#define TREE_SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Node structure for scanned files and directories
 */
typedef struct {
    int id;
    char name[2000];
    char path[2000];
    bool is_dir;
} Node;

extern Node node_list[1000];
extern int node_count;

void traverse(const char *path);

#endif // TREE_SCANNER_H
