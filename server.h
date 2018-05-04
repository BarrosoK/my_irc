/*
** EPITECH PROJECT, 2018
** vector
** File description:
** server.h
*/

#ifndef VECTOR_SERVER_H
#define VECTOR_SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include "vector.h"

typedef struct server_s server_t;

enum server_type
{
	THREAD,
	FORK
};

struct conn_s
{
	server_t *server;
	int conn_fd;
	struct sockaddr_in *sockconn;
};

struct server_s
{
	vector_t *connfd;
	int sockfd;
	int listen;
	char buffer[4096 + 1];
	struct sockaddr_in sockaddr;
	int (*open_port)(server_t *, size_t);
	int (*start_listening)(server_t *, int, enum server_type, void *(*func)(void *));
	int (*get_paquet)(server_t *, int);
};

int start_listening(server_t *, int max_conns, enum server_type, void *(*func)(void *));
int open_port(server_t *, size_t);
int get_paquet(server_t *, int);

server_t *new_server();

#endif //VECTOR_SERVER_H
