#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "inc/vector.h"
#include "inc/server.h"

static void send_all(int my_fd, vector_t *list, char *buffer)
{
	for (size_t i = 0; i < list->size; i++) {
		if (my_fd !=
			(int)list->at(list, i))
			dprintf((int)list->at(list,
				i), "-> %s", buffer);
	}
}

static void *send_msg_to_all(void *para)
{
	struct conn_s *conn = (struct conn_s *)para;
	server_t *server = conn->server;

	while (server->get_paquet(server, conn->conn_fd) == 0) {
		if (strcasecmp(server->buffer, "list\n") == 0) {
			dprintf(conn->conn_fd, "Players connected : %d\n", (int)server->connfd->size);
			continue;
		} else if (strcasecmp(server->buffer, "quit\n") == 0) {
			dprintf(conn->conn_fd, "Disconnected\n");
			break;
		}
		send_all(conn->conn_fd, server->connfd, server->buffer);
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
