/*
** EPITECH PROJECT, 2018
** vector
** File description:
** server.c
*/

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <pthread.h>
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

int get_paquet(server_t *server, int conn_fd)
{
	bzero(server->buffer, 4097);
	if (read(conn_fd, server->buffer, 4096) <= 0) {
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
		server->connfd->push_back(server->connfd, (void *)conn->conn_fd);
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
		server->connfd->push_back(server->connfd, (void *)conn->conn_fd);
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

server_t *new_server()
{
	server_t *server = malloc(sizeof(server_t));

	if (!server)
		return (NULL);
	server->connfd = new_vector();
	server->open_port = open_port;
	server->start_listening = start_listening;
	server->get_paquet = get_paquet;
	return (server);
}
