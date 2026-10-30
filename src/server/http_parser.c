#include "server/server_definitions.h"

int parse_request(const char* buffer, struct http_request *request){
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