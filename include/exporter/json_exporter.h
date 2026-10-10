#ifndef JSON_EXPORTER_H
#define JSON_EXPORTER_H
#include "matrix/matrix.h"

void export_nodes_json(const char *output_filepath);
void export_metrices_json(const MatrixCollection* collection, const char *output_filepath);

#endif // JSON_EXPORTER_H
