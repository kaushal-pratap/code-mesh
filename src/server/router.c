#include "server/server_definitions.h"

void initialize_available_files(struct available_files* available_files){

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

void routing(struct http_request request, struct available_files available_files, struct routing_context *routing_context){
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