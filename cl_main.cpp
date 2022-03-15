#include <string.h>
#include <iostream>
#include <string>
#include <vector>

#include "network/network.h"

#define PORT 9002
#define BUF_MAXL 4096

using  namespace std;

int main(int argc, char** argv)
{
	int csock, qflag;
	string server_message;
	string sv_ip = "prim.staz.io";
	
	if(argc != 2)
	{
		cout << "Argument count incorrect" << endl;
		exit(EXIT_FAILURE);
	}


	csock = create_connection_tcp(PORT, sv_ip);
	server_message = receive_message(csock);
	cout << server_message << endl;
	
	string server_cmd(argv[1]);
	send_message(csock, server_cmd);
	receive_message(csock);

	close_connection(csock);
	return 0;
}
