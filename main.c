#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "vector.h"
#include "server.h"

void *listen_paquet(void *para)
{
	struct conn_s *conn = (struct conn_s *)para;
	server_t *server = conn->server;

	while (server->get_paquet(server, conn->conn_fd) == 0) {
		for (size_t i = 0; i < server->connfd->size; i++) {
			dprintf((int)server->connfd->at(server->connfd, i), "-> %s", server->buffer);
		}
	}
	printf("Client disconnected\n");
}

int main()
{
	vector_t *vector = new_vector();
	server_t *server = new_server();

	vector->push_back(vector, (void *)server);

	server->open_port(server, 30000);
	server->start_listening(server, 5, THREAD, listen_paquet);

	return (0);
}
