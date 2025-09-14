#include "server.h"

// Lit la requête HTTP du client
int read_request(int client_fd, char *buffer, size_t bufsize) {

	memset(buffer, 0, bufsize);

	int bytes_received = recv(client_fd, buffer, bufsize - 1, 0);

	if (bytes_received < 0) {
		printf("Erreur lors de la lecture de la requête: %s\n", strerror(errno));
	}

	return bytes_received;
}

// Envoie une réponse HTTP simple
void answer_request(int client_fd) {
	const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nHello, World!";
	send(client_fd, response, strlen(response), 0);
}