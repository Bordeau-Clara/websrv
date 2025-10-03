#include <sys/types.h>
#include <sys/socket.h>

int main(void) {

	// AF_INET pour internet
	// SOCK_STREAM Needs to be in a connected state
	int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);

	return 0;
}
