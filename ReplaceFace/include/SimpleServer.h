#ifndef SIMPLESERVER_H
#define SIMPLESERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstdio>


class SimpleServer { 

	int listenSocket, clientSocket, status;
	struct sockaddr_in myaddr, cliaddr;
	unsigned int clientSocketLength;

	char recBuffer[2048];

	void closeListenSocket();

public:

	SimpleServer();
	~SimpleServer();

	bool ok();

	int bindWithPort(short port);
	bool listenAndConnect();
	std::string readClient();
	int writeClient(std::string in);
	void closeClient();

};

#endif
