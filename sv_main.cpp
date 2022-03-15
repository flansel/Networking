#include <iterator>
#include <string.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <fstream>
#include <algorithm>
#include <thread>

#include <arpa/inet.h>

#include "network/network.h"

#define PORT 9002
#define BUF_MAXL 4096

using namespace std;

int process(int, struct sockaddr_in);

int main(int argc, char** argv)
{
	int main_sock, new_sock;
	struct sockaddr_in client_info, server_info;
	main_sock = server_open_connection_tcp(PORT, &server_info);

	while(true)
	{
		new_sock = server_accept_connection_tcp(main_sock, &server_info, &client_info);
		process(new_sock, client_info);
		close_connection(new_sock);
	}
}

int process(int csock, struct sockaddr_in client_info)
{
	string message = "Connection established processing request...";
	string client_message, cmp, keycode, ip_handoff_str, tmp;
	char client_ip_str[INET_ADDRSTRLEN];
	ofstream server_list;
	ifstream server_list_read;
	
	send_message(csock, message);
	
	client_message = receive_message(csock);
	cout << client_message << endl;

	keycode = client_message.substr(client_message.find_last_of(" ")+1, client_message.length());
	cmp = client_message.substr(0, client_message.find_first_of(" "));		
	transform(cmp.begin(), cmp.end(), cmp.begin(), [](unsigned char c) -> unsigned char{return tolower(c);});	
	cout << cmp << endl;
	
	if(cmp.compare("start") == 0)
	{
		send_message(csock, "1");
		client_message = receive_message(csock);
		
		if(client_message.compare("2") != 0)
		{
			return -1;
		}

		server_list.open("ledger.txt");
		inet_ntop(AF_INET, &client_info, client_ip_str, INET_ADDRSTRLEN);
		server_list << keycode << " " << client_ip_str << "\n";
		server_list.close();
		send_message(csock, "3");
	}
	else if(cmp.compare("join") == 0)
	{
		server_list_read.open("ledger.txt");
		while(getline(server_list_read, cmp))
		{
			tmp = cmp.substr(0, cmp.find_first_of(" ")+1);
			if(keycode.compare(tmp) == 0)
			{
				ip_handoff_str = "4" + cmp.substr(cmp.find_last_of(" ")+1 ,cmp.length());
				send_message(csock, ip_handoff_str);
				break;
			}
			return 0;
		}
		server_list_read.close();
	}
	else
	{
		send_message(csock, "Command not recognized..Goodbye");
	}
	return 0;
}
