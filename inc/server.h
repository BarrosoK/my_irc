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

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define WARNING "\x1b[33m/\x1b[31m!\x1b[33m\\ \x1b[0m"
#define LOG "\x1b[33m[\x1b[32m-\x1b[33m]\x1b[0m"
#define JOIN "\x1b[33m[\x1b[31m+\x1b[33m]\x1b[0m"
#define PM "\x1b[33m[\x1b[32mPM\x1b[33m]\x1b[0m"
#define QUIT "\x1b[33m[\x1b[31m-\x1b[33m]\x1b[0m"

typedef struct server_s server_t;

enum server_type {
	THREAD,
	FORK
};

struct conn_s {
	server_t *server;
	char *pseudo;
	int conn_fd;
	struct sockaddr_in *sockconn;
};

struct server_s {
	vector_t *connfd;
	vector_t *conn;
	int sockfd;
	int listen;
	char buffer[4096 + 1];
	struct sockaddr_in sockaddr;

	int (*open_port)(server_t *, size_t);
	int (*start_listening)(server_t *, int, enum server_type, void *(*func)(void *));
	int (*get_paquet)(struct conn_s *);
	void (*delete_client)(struct conn_s *);
	void (*ask_pseudo)(struct conn_s *);
	void (*send_list)(struct conn_s *);
	void (*send_reply_all)(struct conn_s *);
	int (*send_pm)(struct conn_s *);
	int (*get_fd_by_name)(server_t *, char *);
};

int get_fd_by_name(server_t *, char *);
int send_pm(struct conn_s *);
void send_reply_all(struct conn_s *);
void send_list(struct conn_s *);
void ask_pseudo(struct conn_s *);
void delete_client(struct conn_s *);
int start_listening(server_t *, int max_conns, enum server_type,
	void *(*func)(void *));
int open_port(server_t *, size_t);
int get_paquet(struct conn_s *);
server_t *new_server();

#endif //VECTOR_SERVER_H
