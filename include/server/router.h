#ifndef ROUTER_H
#define ROUTER_H

void initialize_available_files(struct available_files* available_files);
void routing(struct http_request request, struct available_files available_files, struct routing_context *routing_context);
#endif