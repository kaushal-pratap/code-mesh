#include "server/server_definitions.h"

int construct_response(struct response_context* response_context, struct routing_context routing_context){
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