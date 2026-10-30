#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "server/server_definitions.h"

int parse_request(const char* buffer, struct http_request *request);

#endif