#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "inc/vector.h"
#include "inc/server.h"

static void *connection_handler(void *para)
{
	struct conn_s *conn = (struct conn_s *)para;
	server_t *server = conn->server;

	server->conn->push_back(server->conn, (void *)conn);
	server->ask_pseudo(conn);
	while (server->get_paquet(conn) == 0) {
		if (strcasecmp(server->buffer, "list\n") == 0) {
			server->send_list(conn);
			continue;
		} else if (strcasecmp(server->buffer, "quit\n") == 0) {
			break;
		} else if (strncasecmp(server->buffer, "pm", 2) == 0) {
			server->send_pm(conn);
			continue;
		}
		server->send_reply_all(conn);
	}
	server->delete_client(conn);
	return (NULL);
}

int main()
{
	server_t *server = new_server();

	server->open_port(server, 30000);
	server->start_listening(server, 5, THREAD, connection_handler);
	return (0);
}
