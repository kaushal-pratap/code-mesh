#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include "server/server_definitions.h"

int initialize_server(struct server_internals* server_internals);
int receive_bytes(struct receive_bytes_context* receive_bytes_context, const int client_fd);
int send_all(int client_fd, const char* data, size_t length);
#endif