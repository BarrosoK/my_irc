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

void ask_pseudo(struct conn_s *conn)
{
	server_t *server = conn->server;

	dprintf(conn->conn_fd, "Enter your pseudo : ");
	conn->server->get_paquet(conn->server, conn->conn_fd);
	conn->pseudo = strdup(strtok(conn->server->buffer, "\n"));
	for (size_t i = 0; i < server->conn->size; i++) {
		struct conn_s *temp = (struct conn_s *)server->conn->at(server->conn, i);
		if (conn->conn_fd != temp->conn_fd)  {
			dprintf(temp->conn_fd, "%s joined\n", conn->pseudo);
		} else {
			dprintf(conn->conn_fd, "Welcome %s\n", conn->pseudo);
		}
	}
}

void delete_client(struct conn_s *conn)
{
	server_t *server = conn->server;
	size_t id = 0;

	for (size_t i = 0; i < server->conn->size; i++) {
		struct conn_s *temp = (struct conn_s *)server->conn->at(server->conn, i);
		if (conn->conn_fd == temp->conn_fd) {
			id = i;
			dprintf(conn->conn_fd, "Disconnected\n");
		} else {
			dprintf(temp->conn_fd, "%s disconnected\n", conn->pseudo);
		}
	}
	server->conn->erase(server->conn, id);
	server->connfd->erase(server->connfd, id);
	free(conn);
}

void send_list(server_t *server, int fd)
{
	dprintf(fd, "Players connected : %d\n", (int)server->connfd->size);
	for (size_t i = 0; i < server->conn->size; i++) {
		struct conn_s *temp = (struct conn_s *)server->conn->at(server->conn, i);
		dprintf(fd, "%d : %s\n", temp->conn_fd, temp->pseudo);
	}
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
	return (server);
}
