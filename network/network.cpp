#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#define PORT 8080
#define BUF_MAXL 4096

using  std::cout, std::endl, std::vector, std::string;


/*
 * Takes in an active socket and a message and sends the message through
 * the socket. Returns 0 on an error, and 1 on success.
 */
int send_message(int sock, string message)
{
		if(send(sock, message.c_str(), message.length(), 0) < 0)
		{
				return 0;
		}

		return 1;
}

/*
 * Takes in an active socket and blocks until it receives a message, the message in then
 * returned as a string.
 */
string receive_message(int sock)
{
	string ret;
	int bytes;
	vector<char> buffer(BUF_MAXL);
	fill(buffer.begin(), buffer.end(), 0);

	do
	{
		bytes = recv(sock, &buffer[0], buffer.size(), 0); 
		
		for(char c : buffer)
		{
			if(c == '\0')
			{
				break;
			}
			ret += c;
		}
	}
	while(bytes == BUF_MAXL);
	return ret;
}

/*
 * For client use, creates a socket with addresss addr and port port, the socket is returned
 */
int create_connection_tcp(int port, string addr)
{
	int sock = 0;
	struct sockaddr_in address;
	struct hostent* server;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "An Error occured while trying to create the socket" << endl;
		exit(EXIT_FAILURE);
	}
	
	server = gethostbyname(addr.c_str());

	if(server == NULL)
	{
		perror("Get host name");
		exit(EXIT_FAILURE);
	}

	bzero((char*)&address, sizeof(address));
	address.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&address.sin_addr.s_addr, server->h_length);
	address.sin_port = htons(port);

	if(connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		perror("Connection");
		exit(EXIT_FAILURE);
	}

	return sock;
}

/*
 * For server use, starts a server listening on port port, then returns the active socket to
 * be used.
 */
int server_open_connection_tcp(int port, struct sockaddr_in* server_address)
{
	int sock, opt=1;
	struct sockaddr_in address;


	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "An error occured while creating the socket" << endl;
		exit(EXIT_FAILURE);
	}

	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		cout << "An error occured while setting socket options" << endl;
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	if(bind(sock, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		cout << "An error occured while binding the socket" << endl;
		exit(EXIT_FAILURE);
	}

	if(listen(sock, 10) < 0)
	{
		cout << "Error while listening" << endl;
		exit(EXIT_FAILURE);
	}

	memcpy(server_address, &address, sizeof(struct sockaddr_in));
	return sock;
}

int server_accept_connection_tcp(int sock, struct sockaddr_in* address, struct sockaddr_in* client_inf)
{
	int addr_len = sizeof(*address);
	int new_socket;
	if((new_socket = accept(sock, (struct sockaddr*)&address, (socklen_t*)&addr_len)) < 0)
	{
		cout << "Error while accepting" << endl;
		exit(EXIT_FAILURE);
	}

	if(client_inf != NULL)
	{
		memcpy(client_inf, &address, sizeof(struct sockaddr_in));
	}

	return new_socket;
}

void close_connection(int sock)
{
	close(sock);
}
