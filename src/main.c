#include "scanner/tree_scanner.h"
#include "exporter/json_exporter.h"
#include "matrix/matrix.h"
#include "ast/ast_traversal.h"
#include <stdio.h>
#include <string.h>

static int is_c_file(const char *name) {
    size_t len = strlen(name);
    if (len < 3) return 0;
    return (name[len - 2] == '.' && name[len - 1] == 'c');
}

int main(int argc, char *argv[]) {

    MatrixCollection collection = {
        .matrices = NULL,
        .count = 0,
        .capacity = 2
    };

    const char *dir = (argc > 1) ? argv[1] : ".";

    printf("==========================================\n");
    printf("        Code Mesh — C Systems Engine      \n");
    printf("==========================================\n\n");

    // --- Phase 1: Scan the directory tree ---
    printf("--> Scanning directory tree: %s...\n", dir);
    traverse(dir);

    // --- Phase 2: Export tree to JSON ---
    printf("--> Exporting %d nodes to web/mesh_data.json...\n", node_count);
    export_nodes_json("web/mesh_data.json");

    // --- Phase 3: Analyze each .c file for 2D matrices ---
    printf("\n--> Analyzing C source files for 2D matrices...\n");
    int total_matrices = 0;

    for (int i = 0; i < node_count; i++) {
        if (!node_list[i].is_dir && is_c_file(node_list[i].name)) {
            printf("    Parsing: %s\n", node_list[i].path);
            int found = analyze_file(node_list[i].path, &collection);
            if (found > 0) {
                total_matrices += found;
            }
        }
    }

    printf("\n--> Total matrices discovered: %zu (stored in collection)\n", collection.count);
    printf("\n--> Success! Code Mesh pipeline complete.\n");
    return 0;
}
