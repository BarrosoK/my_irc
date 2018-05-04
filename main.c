#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "vector.h"
#include "server.h"

void *send_msg_to_all(void *para)
{
	struct conn_s *conn = (struct conn_s *)para;
	server_t *server = conn->server;

	while (server->get_paquet(server, conn->conn_fd) == 0) {
		if (strcasecmp(server->buffer, "list\n") == 0) {
			dprintf(conn->conn_fd, "Players connected : %d\n", (int)server->connfd->size);
			continue;
		}
		for (size_t i = 0; i < server->connfd->size; i++) {
			if (conn->conn_fd !=
				(int)server->connfd->at(server->connfd, i))
				dprintf((int)server->connfd->at(server->connfd,
					i), "-> %s", server->buffer);
		}
	}
	printf("Client disconnected\n");
	free(conn);
	return (NULL);
}

int main()
{
	server_t *server = new_server();

	server->open_port(server, 30000);
	server->start_listening(server, 5, THREAD, send_msg_to_all);
	return (0);
}
