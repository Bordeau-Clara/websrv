#include <string>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int ARGS = 3;

typedef struct server {
	const char *hostName;
	const char *portNumber;
} t_server;

int clientSocket(t_server *server) {

	return 0;
}

int main(int argc, char **argv) {

	if (argc != ARGS) {
		std::cerr << "UsaGE: ./server <> <>" << std::endl;
		return 1;
	}

	t_server server = {
		.hostName = argv[1],
		.portNumber = argv[2]
	};

	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));

	struct addrinfo *record;
	struct addrinfo *results;

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(server.hostName, server.portNumber, &hints, &results) != 0) {
		std::cerr << "Can't get adrr info" << std::endl;
		exit(1);
	}

	int clientSocket;

	for (record = results; record != NULL; record = record->ai_next) {
		clientSocket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
		if (clientSocket == -1) 
			continue ;
		if (connect(clientSocket, record->ai_addr, record->ai_addrlen) != -1)
			break ;
		close(clientSocket);
	}

	if (record == NULL) {
		std::cerr << "Failed to create or connect client socket" << std::endl;
		exit(-1);
	}

	freeaddrinfo(results);

	while (true) {

		char *message;
		std::cin >> message;

		if (send(clientSocket, message, strlen(message), 0) == -1) {
			exit(-1);
		}
	}
	
	close(clientSocket);
	return 0;
}
