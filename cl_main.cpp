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
	string sv_ip = "96.126.108.8";
	string message = "The client is contacting the server...Congrats";
	
	csock = create_connection_tcp(PORT, sv_ip);
	send_message(csock, message);
	server_message = receive_message(csock, qflag);

	cout << server_message << endl;

	while(true)
	{
		getline(cin, message);
		if(message.compare("quit") == 0)
			break;

		send_message(csock, message);
	}

	close_connection(csock);
	return 0;
}
