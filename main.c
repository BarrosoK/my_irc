#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "inc/vector.h"
#include "inc/server.h"

static void send_all(struct conn_s *conn)
{
	vector_t *list = conn->server->connfd;

	for (size_t i = 0; i < list->size; i++) {
		if (conn->conn_fd !=
			(int)list->at(list, i))
			dprintf((int)list->at(list,
				i), "%s : %s", conn->pseudo ,conn->server->buffer);
	}
}

static void *send_msg_to_all(void *para)
{
	struct conn_s *conn = (struct conn_s *)para;
	server_t *server = conn->server;

	server->conn->push_back(server->conn, (void *)conn);
	server->ask_pseudo(conn);
	while (server->get_paquet(server, conn->conn_fd) == 0) {
		if (strcasecmp(server->buffer, "list\n") == 0) {
			server->send_list(server, conn->conn_fd);
			continue;
		} else if (strcasecmp(server->buffer, "quit\n") == 0) {
			break;
		}
		send_all(conn);
	}
	server->delete_client(conn);
	return (NULL);
}

int main()
{
	server_t *server = new_server();

	server->open_port(server, 30000);
	server->start_listening(server, 5, THREAD, send_msg_to_all);
	return (0);
}
