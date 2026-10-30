#include "server/server_definitions.h"

int initialize_server(struct server_internals* server_internals){
    server_internals->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_internals->socket_fd == -1){
        perror("socket");
        return -1;
    }
    printf("Socket created successfully FD = %d\n", server_internals->socket_fd);
    struct sockaddr_in address = {0};
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(bind(server_internals->socket_fd, (const struct sockaddr*)&address, sizeof(address)) == -1){
        perror("bind");
        close(server_internals->socket_fd);
        return -1;
    }
    printf("Socket bound to 127.0.0.1:8080\n");

    if(listen(server_internals->socket_fd, 5) == -1){
        perror("listen");
        close(server_internals->socket_fd);
        return -1;
    }
    printf("Server is listening on 127.0.0.1:8080\n");

    return 0;
}

int receive_bytes(struct receive_bytes_context* receive_bytes_context, const int client_fd){
    while(strstr(receive_bytes_context->buffer,"\r\n\r\n") == NULL){
        
        if(receive_bytes_context->total_received >= sizeof(receive_bytes_context->buffer) - 1){
            fprintf(stderr, "Header receive limit reached\n");
            return -1;
        }
        ssize_t bytes_received = recv(client_fd, receive_bytes_context->buffer + receive_bytes_context->total_received, sizeof(receive_bytes_context->buffer) - 1 - receive_bytes_context->total_received, 0);

        if(bytes_received <= 0){
            fprintf(stderr,"Client disconnected without sending data.\n");
            return -1;
        }
        receive_bytes_context->total_received += bytes_received;
        receive_bytes_context-> buffer[receive_bytes_context->total_received] = '\0';
    }
    if (strstr(receive_bytes_context-> buffer, "\r\n\r\n") != NULL) {
        receive_bytes_context->request_complete = 1;
    }
    if (!receive_bytes_context->request_complete) {
        fprintf(stderr,"Header request incomplete.\n");
        return -1;
    }
    printf("Received %zd bytes\n", receive_bytes_context->total_received);
    return 0;
}

int send_all(int client_fd, const char* data, size_t length){
    size_t offset = 0;
    while(offset < length){
        ssize_t bytes_sent = send(client_fd, data+offset, length-offset, 0);
        if(bytes_sent == -1 || bytes_sent == 0){
            printf("Sent %zd bytes\n", bytes_sent);
            perror("send");
            return -1;
        }
        else{
            offset += bytes_sent;
        }
    }
    return 0;
}
