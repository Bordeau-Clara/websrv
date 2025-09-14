#include "server.h"

int main() {
	// Disable output buffering
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	int client_addr_len;
	struct sockaddr_in client_addr;

	int server_fd = setup_server(4221, 5);

	if (server_fd < 0) {
		return 1;
	}

	client_addr_len = sizeof(client_addr);
	int id = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
	if (id < 0) {
		printf("Accept failed: %s \n", strerror(errno));
		close(server_fd);
		return 1;
	}

	char request_buffer[REQ_BUF_SIZE];
	int bytes_received = read_request(id, request_buffer, REQ_BUF_SIZE);

	if (bytes_received < 0) {
		close(id);
		close(server_fd);
		return 1;
	}

	answer_request(id);

	close(id);
	close(server_fd);

	return 0;
}
