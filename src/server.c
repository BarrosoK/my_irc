/*
** EPITECH PROJECT, 2018
** vector
** File description:
** server.c
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <pthread.h>
#include <memory.h>
#include "../inc/server.h"

int open_port(server_t *server, size_t port)
{
	server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->sockfd < 0)
		return (-1);
	if (setsockopt(server->sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1},
		sizeof(int)) < 0) {
		return (-1);
	}
	bzero((char *)&server->sockaddr, sizeof(server->sockaddr));
	server->sockaddr.sin_family = AF_INET;
	server->sockaddr.sin_addr.s_addr = INADDR_ANY;
	server->sockaddr.sin_port = htons((uint16_t)port);
	if (bind(server->sockfd, (struct sockaddr *)&server->sockaddr,
		sizeof(server->sockaddr)) < 0)
		return (-1);
	server->listen = 0;
	return (0);
}

int get_paquet(struct conn_s *conn)
{
	server_t *server = conn->server;

	bzero(server->buffer, 4097);
	if (read(conn->conn_fd, server->buffer, 4096) <= 0) {
		return (-1);
	}
	return (0);
}

static void listen_thread(server_t *server, void *(*func)(void *))
{
	struct conn_s *conn;
	socklen_t size = sizeof(conn->sockconn);
	pthread_t thread_id;

	while (server->listen) {
		conn = malloc(sizeof(struct conn_s));
		conn->server = server;
		conn->conn_fd = accept(server->sockfd,
			(struct sockaddr *)conn->sockconn, &size);
		pthread_create(&thread_id, NULL, func, (void *)conn);
	}
}

static void listen_fork(server_t *server, void *(*func)(void *))
{
	struct conn_s *conn;
	socklen_t size = sizeof(conn->sockconn);

	while (server->listen) {
		conn = malloc(sizeof(struct conn_s));
		conn->server = server;
		conn->conn_fd = accept(server->sockfd,
			(struct sockaddr *)conn->sockconn, &size);
		if (fork() == 0) {
			func(conn);
		}
	}
}

int start_listening(server_t *server, int max_connections,
	enum server_type type, void *(*func)(void *))
{
	listen(server->sockfd, max_connections);
	server->listen = 1;
	if (type == FORK) {
		listen_fork(server, func);
	} else {
		listen_thread(server, func);
	}
	return (0);
}

void ask_pseudo(struct conn_s *conn)
{
	server_t *server = conn->server;

	while (!conn->pseudo) {
		dprintf(conn->conn_fd, "%s Enter your pseudo : ", LOG);
		conn->server->get_paquet(conn);
		if (get_fd_by_name(conn->server, strtok(conn->server->buffer, "\n")) == -1)
			break;
		dprintf(conn->conn_fd, "%s Name already taken\n", WARNING);
	}
	if (!conn->server->buffer || strlen(conn->server->buffer) == 0)
		return ;
	conn->pseudo = strdup(strtok(conn->server->buffer, " "));
	for (size_t i = 0; i < server->conn->size; i++) {
		struct conn_s *temp = (struct conn_s *)server->conn->at(
			server->conn, i);
		if (conn->conn_fd != temp->conn_fd) {
			dprintf(temp->conn_fd, "%s %s joined\n", JOIN, conn->pseudo);
		}
	}
	dprintf(conn->conn_fd, "%s Welcome %s\n", JOIN, conn->pseudo);
}

void delete_client(struct conn_s *conn)
{
	server_t *server = conn->server;
	size_t id = 0;

	for (size_t i = 0; i < server->conn->size; i++) {
		struct conn_s *temp = (struct conn_s *)server->conn->at(
			server->conn, i);
		if (conn->conn_fd == temp->conn_fd) {
			id = i;
			dprintf(conn->conn_fd, "%s Disconnected\n", QUIT);
		} else {
			dprintf(temp->conn_fd, "%s %s disconnected\n", QUIT,
				conn->pseudo);
		}
	}
	server->conn->erase(server->conn, id);
	server->connfd->erase(server->connfd, id);
	free(conn);
}

void send_reply_all(struct conn_s *conn)
{
	vector_t *list = conn->server->conn;
	int fd;

	for (size_t i = 0; i < list->size; i++) {
		fd = ((struct conn_s *)list->at(list, i))->conn_fd;
		if (conn->conn_fd != fd)
			dprintf(fd, "%s[%s%s%s]%s %s", ANSI_COLOR_YELLOW, ANSI_COLOR_CYAN, conn->pseudo, ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, conn->server->buffer);
	}
}

void send_list(struct conn_s *conn)
{
	server_t *server = conn->server;

	dprintf(conn->conn_fd, "%s Players connected : %d\n", LOG,
		(int)server->conn->size);
	for (size_t i = 0; i < server->conn->size; i++) {
		struct conn_s *temp = (struct conn_s *)server->conn->at(
			server->conn, i);
		dprintf(conn->conn_fd, "%s[%s%d%s]%s : %s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_GREEN, (int)i, ANSI_COLOR_YELLOW, ANSI_COLOR_RESET,
			temp->pseudo);
	}
}


int send_pm(struct conn_s *conn)
{
	server_t *server = conn->server;
	char *receiver_pseudo;
	int receiver_fd;
	char *msg;

	strtok(server->buffer, " ");
	receiver_pseudo = strtok(NULL, " ");
	if (!receiver_pseudo)
		return (-1);
	receiver_fd = server->get_fd_by_name(server, receiver_pseudo);
	msg = strtok(NULL, "\n");
	dprintf(receiver_fd, "%s%s[%s%s%s]%s : %s\n", PM, ANSI_COLOR_YELLOW, ANSI_COLOR_CYAN, conn->pseudo, ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, msg);
	return (0);
}

int get_fd_by_name(server_t *server, char *name)
{
	struct conn_s *conn;

	if (!name)
		return (-1);
	for (size_t i = 0; i < server->conn->size; i++) {
		conn = server->conn->at(server->conn, i);
		if (strcmp(conn->pseudo, name) == 0) {
			return (conn->conn_fd);
		}
	}
	return (-1);
}

server_t *new_server()
{
	server_t *server = malloc(sizeof(server_t));

	if (!server)
		return (NULL);
	server->connfd = new_vector();
	server->conn = new_vector();
	server->open_port = open_port;
	server->start_listening = start_listening;
	server->get_paquet = get_paquet;
	server->delete_client = delete_client;
	server->ask_pseudo = ask_pseudo;
	server->send_list = send_list;
	server->send_reply_all = send_reply_all;
	server->send_pm = send_pm;
	server->get_fd_by_name = get_fd_by_name;
	return (server);
}
