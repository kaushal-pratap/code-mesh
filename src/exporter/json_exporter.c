#include "exporter/json_exporter.h"
#include "scanner/tree_scanner.h"
#include "matrix/matrix.h"
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

void export_metrices_json(const MatrixCollection* collection, const char *output_filepath){
  int fd = open(output_filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd == -1) {
    perror("Unable to open JSON output file");
    return;
  }

  dprintf(fd, "{\n  \"matrices\": [\n");

  for(int i = 0; i < collection->count; i++){

    dprintf(fd, "    {\n");
    dprintf(fd, "      \"name\": \"%s\",\n", collection->matrices[i]->name);
    dprintf(fd, "      \"rows\": %zu,\n", collection->matrices[i]->rows);
    dprintf(fd, "      \"columns\": %zu,\n", collection->matrices[i]->columns);
    dprintf(fd, "      \"data\": [\n");


    for(int row = 0; row < collection->matrices[i]->rows; row++){

      dprintf(fd, "        [");

      for(int column = 0; column < collection->matrices[i]->columns; column++){

        Matrix matrix = *(collection->matrices[i]);
        MatrixType matrix_type = matrix.type;
        size_t index = (row * matrix.columns) + column;

        if (matrix_type == MATRIX_INT) {
          int value;
          matrix_get(&matrix, index, &value);
          dprintf(fd, "%d", value);
        } 
        else if (matrix_type == MATRIX_FLOAT) {
          float value;
          matrix_get(&matrix, index, &value);
          dprintf(fd, "%f", value);
        }
        else if(matrix_type == MATRIX_DOUBLE){
          double value;
          matrix_get(&matrix, index, &value);
          dprintf(fd, "%lf", value);
        }
        else if (matrix_type == MATRIX_CHAR){
          char value;
          matrix_get(&matrix, index, &value);
          dprintf(fd, "%c", value);
        }
        if (column < matrix.columns - 1) {
          dprintf(fd, ", ");
        }

      }

      dprintf(fd, "]%s\n", (row == collection->matrices[i]->rows - 1) ? "" : ",");

    }

    dprintf(fd, "      ]\n    }%s\n", (i == collection->count - 1) ? "" : ",");

  }
  
  dprintf(fd, "  ]\n}\n");

  close(fd);
}


