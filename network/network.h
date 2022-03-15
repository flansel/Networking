#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

using std::string;

int send_message(int sock, string message);
string receive_message(int sock);
int create_connection_tcp(int port, string addr);
int server_open_connection_tcp(int port, struct sockaddr_in* server_info);
int server_accept_connection_tcp(int port, struct sockaddr_in* server_info, struct sockaddr_in* client_inf);
void close_connection(int sock);
#endif
