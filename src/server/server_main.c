#include "server/server_definitions.h"
#include "server/http_server.h"
#include "server/http_parser.h"
#include "server/http_response.h"
#include "server/router.h"

int main(void){
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
            return -1;
        }
     

        if (send_all(server.client_fd, response_ctx.response, response_ctx.response_length) == -1) {
            close(server.client_fd);
            continue;
        }
        close(server.client_fd);
        
    }
    close(server.socket_fd);
    return 0;
}