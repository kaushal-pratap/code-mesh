#include "exporter/json_exporter.h"
#include "scanner/tree_scanner.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void export_nodes_json(const char *output_filepath) {
  int fd = open(output_filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("Unable to open JSON output file");
    return;
  }

  dprintf(fd, "{\n  \"tree\": [\n");

  for (int i = 0; i < node_count; i++) {
    dprintf(fd, "    {\n");
    dprintf(fd, "      \"id\": %d,\n", node_list[i].id);
    dprintf(fd, "      \"name\": \"%s\",\n", node_list[i].name);
    dprintf(fd, "      \"path\": \"%s\",\n", node_list[i].path);
    dprintf(fd, "      \"is_dir\": %s\n", node_list[i].is_dir ? "true" : "false");
    dprintf(fd, "    }%s\n", (i == node_count - 1) ? "" : ",");
  }

  dprintf(fd, "  ]\n}\n");
  close(fd);
}
