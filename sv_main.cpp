#include <iterator>
#include <string.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <fstream>
#include <algorithm>

#include <arpa/inet.h>

#include "network/network.h"

#define PORT 9002
#define BUF_MAXL 4096

using namespace std;


int main(int argc, char** argv)
{
	int csock, qflag;
	struct sockaddr_in client_info;
	string message = "You've conected with the server...Congrats";
	string client_message;
	string cmp;
	char client_ip_str[INET_ADDRSTRLEN];
	ofstream server_list;
	server_list.open("ledger.txt");
	
	csock = server_open_connection_tcp(PORT, &client_info);
	client_message = receive_message(csock, qflag);

	cout << client_message << endl;

	send_message(csock, message);
	
	while(true)
	{
		client_message = receive_message(csock, qflag);
		if(qflag == 1)
			break;
		
		cmp = client_message.substr(0, client_message.find_first_of(" "));
		transform(cmp.begin(), cmp.end(), cmp.begin(), [](unsigned char c) -> unsigned char{return tolower(c);});
		
		cout << cmp << endl;
		cout << client_message.length() << endl;
		if(cmp.compare("start") == 0)
		{
			//Add a server to the ledger (ip, key)
			//Tell the client they are ok to start a udp server.
			//close the connection
			inet_ntop(AF_INET, &client_info, client_ip_str, INET_ADDRSTRLEN);
			server_list << client_message.substr(client_message.find_last_of(" ")+1, client_message.length()) << " " << client_ip_str << "\n";

		}
		else if(cmp.compare("join") == 0)
		{
			//Check for the server in the ledger
			//if it exists then give the client the ip and the ok to connect and close the socket
			//else tell if it doesnt exist
			cout << "in progress" << endl;
		}
		else
		{
			cout << client_message << endl;
		}
		
	}
	
	close_connection(csock);

	return 0;
}
