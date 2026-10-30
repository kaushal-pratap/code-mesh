#ifndef SERVER_DEFINITIONS_H
#define SERVER_DEFINITIONS_H

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

typedef struct http_request{
    char method[16];
    char path[256];
    char version[16];
} http_request;

typedef struct receive_bytes_context{
    char buffer[4096];
    size_t total_received;
    int request_complete;

} receive_bytes_context;

typedef struct available_files{
    const char *html_found;
    const char *html_not_found;
}available_files;

typedef struct routing_context{
    const char *status;
    const char *html;
} routing_context;

typedef struct response_context{
    char response[2048];
    int header_length;
    size_t response_length;
} response_context;

typedef struct server_internals{
    int socket_fd;
    int client_fd;
} server_internals;

#endif