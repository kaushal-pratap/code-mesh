#include "scanner/tree_scanner.h"
#include "exporter/json_exporter.h"
#include "matrix/matrix.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    const char *dir = (argc > 1) ? argv[1] : ".";

    printf("==========================================\n");
    printf("        Code Mesh — C Systems Engine      \n");
    printf("==========================================\n\n");

    printf("--> Scanning directory tree: %s...\n", dir);
    traverse(dir);

    printf("--> Exporting %d nodes to web/mesh_data.json...\n", node_count);
    export_nodes_json("web/mesh_data.json");

    printf("\n--> Success! Code Mesh pipeline complete.\n");
    return 0;
}
