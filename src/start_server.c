#include "server.h"

// Crée, configure et met en écoute le socket serveur. Retourne le fd ou -1 en cas d'erreur.
int setup_server(int port, int backlog) {
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		printf("Socket creation failed: %s...\n", strerror(errno));
		return -1;
	}

	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEADDR failed: %s \n", strerror(errno));
		close(server_fd);
		return -1;
	}

	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(port),
									 .sin_addr = { htonl(INADDR_ANY) },
								   };

	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("Bind failed: %s \n", strerror(errno));
		close(server_fd);
		return -1;
	}

	if (listen(server_fd, backlog) != 0) {
		printf("Listen failed: %s \n", strerror(errno));
		close(server_fd);
		return -1;
	}

	return server_fd;
}