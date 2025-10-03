#include <string>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {

	const size_t bufferSize = 1024;
	const char *portNumber = "8080";
	const int backlog = 2;
	int serverSocket;
	struct serverSocket;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	struct addrinfo *record;
	struct addrinfo *results;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(NULL, portNumber, &hints, &results) != 0) {
		std::cerr << "Can't get server's adress" << std::endl;
		exit(-1);
	}

	for (record = results; record != NULL; record = record->ai_next) {
		serverSocket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
		if (serverSocket == -1)
			continue;
		int enable = 1;
		setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
		if (bind(serverSocket, record->ai_addr, record->ai_addrlen) == 0)
			break;
		close(serverSocket);
	}

	freeaddrinfo(results);

	if (listen(serverSocket, backlog) == -1) {
		std::cerr << "I'm deaf" << std::endl;
		exit(1);
	}

	while (1) {

		int clientSocket;
		struct sockaddr clientAdress;
		socklen_t clientAdressLenght = sizeof(clientAdress);

		if ((clientSocket = accept(serverSocket, &clientAdress, &clientAdressLenght)) < 0) {
			std::cerr << "Aled" << std::endl;
			exit(1);
		}

		int received;
			
		char buffer[bufferSize] = {0};
		received = recv(clientSocket, buffer, sizeof(buffer), 0);
		if (received == -1) {
			std::cerr << "I,m blind" << std::endl;
			exit(1);
		} else {
			std::cout << "Message: " << buffer << std::endl;
		}

		close(clientSocket);

	}

	close(serverSocket);
	return 0;
}
