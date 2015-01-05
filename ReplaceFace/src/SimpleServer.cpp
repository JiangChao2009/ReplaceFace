#include "SimpleServer.h"

#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>

using namespace std;

#define BUFFLEN 2048
#define MAX_READ 65536

#define PORT 8000

SimpleServer::SimpleServer() : listenSocket(0), clientSocket(0), status(0) {
	memset((void *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	//myaddr.sin_port = htons(port);

	clientSocketLength = sizeof(cliaddr);
	if ((listenSocket = socket(AF_INET, SOCK_STREAM, 0)) >= 0)
		status = 0;

	int yes = 1;
	setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(int));
}


SimpleServer::~SimpleServer() {
	closeListenSocket();
}


bool SimpleServer::ok() {
	return status == 0 && listenSocket >= 0;
}



int SimpleServer::bindWithPort(short port) {
    myaddr.sin_port = htons(port);
    if (bind(listenSocket, (struct sockaddr *)&myaddr, sizeof(myaddr)) != 0) {
        fprintf(stderr, "Couldn't bind to port %hi: %s\n", port, strerror(errno));
        return 0;
    }
    return port;
}


void SimpleServer::closeListenSocket() {
	if (listenSocket > 0)
		close(listenSocket);
	listenSocket = -1;
	status = -1;
}


bool SimpleServer::listenAndConnect() {
    listen(listenSocket, 1);
	if (ok() && (clientSocket = accept(listenSocket, (struct sockaddr *)&cliaddr, &clientSocketLength)) == -1)
		return (status = -1) == 0;
	return ok();
}


string SimpleServer::readClient() {
	memset(recBuffer, 0, sizeof(recBuffer));
	string message;
	int recSize;
	while ((recSize = recv(clientSocket, recBuffer, sizeof(recBuffer), 0)) > 0) {
		message += recBuffer;
		if (recSize * sizeof(*recBuffer) < sizeof(recBuffer))
			break;
	}
	if (message.size() == 0)
		status = -1;
	return message;
}

int SimpleServer::writeClient(string info) {
    return send(clientSocket, info.c_str(), info.size(), 0);
}

void SimpleServer::closeClient() {
	if (clientSocket > 0) {
		close(clientSocket);
		clientSocket = -1;
	}
	if (listenSocket > 0)
		status = 0;
}
