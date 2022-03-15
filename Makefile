all: client server

client: cl_main.cpp network/network.cpp
	g++ cl_main.cpp network/network.cpp -o client -std=c++17

server: sv_main.cpp network/network.cpp
	g++ sv_main.cpp network/network.cpp -o server -std=c++17

clean:
	rm client server
