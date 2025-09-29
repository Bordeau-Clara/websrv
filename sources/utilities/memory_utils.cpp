#include "webserv.hpp"

void	buffer_flusher(char buffer[], size_t len) {

	for (size_t i = 0; i < len; i++) {
		buffer[i] = 0;
	}
}
