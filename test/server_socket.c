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

static int initialize_server(struct server_internals* server_internals){
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

static int parse_request(const char* buffer, struct http_request *request){
    int fields_parsed = sscanf(buffer, "%15s %255s %15s",request->method, request->path, request->version);
    if (fields_parsed != 3) {
        fprintf(stderr, "Invalid HTTP request line\n");
        return -1;
    }
    printf("Method: %s\n", request->method);
    printf("Path: %s\n", request->path);
    printf("Version: %s\n", request->version);
    return 0;
}

static int receive_bytes(struct receive_bytes_context* receive_bytes_context, const int client_fd){
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

static void initialize_available_files(struct available_files* available_files){

    available_files->html_not_found =
    "<!DOCTYPE html>\r\n"
    "<html><body><h1>404 - Page not found</h1></body></html>\r\n";
    
    available_files->html_found =
    "<!DOCTYPE html>\r\n"
    "<html lang=\"en\">\r\n"
    "<head>\r\n"
    "<meta charset=\"UTF-8\">\r\n"
    "<title>Code Mesh</title>\r\n"
    "</head>\r\n"
    "<body>\r\n"
    "<h1>Code Mesh</h1>\r\n"
    "<p>Your C server is working.</p>\r\n"
    "</body>\r\n"
    "</html>\r\n";
}

static void routing(struct http_request request, struct available_files available_files, struct routing_context *routing_context){
    if(strcmp(request.method, "GET") == 0 && strcmp(request.path, "/") ==0){
        printf("Route : home\n");
        routing_context->status = "200 OK";
        routing_context->html = available_files.html_found;  
    }
    else {
        printf("Route : Not Found\n");
        routing_context->status = "404 Not Found";
        routing_context->html = available_files.html_not_found;
    }
}

static int construct_response(struct response_context* response_context, struct routing_context routing_context){
    response_context->header_length = snprintf(
        response_context->response, 
        sizeof(response_context->response),
        "HTTP/1.1 %s\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        routing_context.status,
        strlen(routing_context.html)
        );
        if (response_context->header_length < 0 ||
            (size_t)response_context->header_length >= sizeof(response_context->response)) {
                fprintf(stderr, "Failed to construct HTTP headers\n");
                return -1;
            }
            size_t body_length = strlen(routing_context.html);
            if ((size_t)response_context->header_length + body_length > sizeof(response_context->response)) {
            fprintf(stderr, "HTTP response is too large\n");
            return -1;
        }
        memcpy(response_context->response + response_context->header_length, routing_context.html, body_length);
        response_context->response_length = (size_t)response_context->header_length + body_length;

        return 0;
}

static int send_all(int client_fd, const char* data, size_t length){
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

int main(){
    server_internals server = {0};
    if(initialize_server(&server) == -1){
        return -1;
    }

    while(1){
        server.client_fd = accept(server.socket_fd, NULL, NULL);
        if(server.client_fd == -1){
            perror("accept");
            continue;
        }
        printf("Client connected successfully!\n");

        receive_bytes_context bytes_ctx = {
            .total_received = 0,
            .request_complete = 0,
            .buffer[0] = '\0'
        };

        if(receive_bytes(&bytes_ctx, server.client_fd) == -1){
            close(server.client_fd);
            continue;
        }
      
        http_request request = {0};
        if(parse_request(bytes_ctx.buffer, &request) == -1){
            close(server.client_fd);
            continue;
        }
      
        available_files files = {0};
        initialize_available_files(&files);
    
        routing_context routing_ctx = {0};
        routing(request, files, &routing_ctx);
  
        response_context response_ctx = {0};
        if(construct_response(&response_ctx, routing_ctx) == -1){
            close(server.client_fd);
            close(server.socket_fd);
            return -1;
        }
     

        if (send_all(server.client_fd, response_ctx.response, response_ctx.response_length) == -1) {
            close(server.client_fd);
            continue;
        }
        
    }
    close(server.socket_fd);
    return 0;
}