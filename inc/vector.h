/*
** EPITECH PROJECT, 2018
** vector
** File description:
** vector.h
*/

#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <stdio.h>

typedef struct v_object_s v_object_t;

typedef struct vector_s vector_t;

enum sort {
	ASCII_ASCEND = 0,
	ASCII_DESCEND
};

struct v_object_s {
	v_object_t *next;
	v_object_t *prev;
	void *data;
};

struct vector_s {
	size_t (*get_size)(vector_t *vector);
	int (*push_back)(vector_t *vector, void *data);
	void *(*at)(vector_t *vector, size_t position);
	int (*empty)(vector_t *vector);
	int (*erase)(vector_t *vector, size_t position);
	int (*pop_back)(vector_t *vector);
	int (*clear)(vector_t *vector);
	int (*insert)(vector_t *vector, size_t position, void *data);
	void *(*front)(vector_t *vector);
	void *(*back)(vector_t *vector);
	int (*sort)(vector_t *vector, enum sort);
	int (*swap)(vector_t *vector, size_t pos1, size_t pos2);
	v_object_t *(*get)(vector_t *vector, size_t position);
	size_t size;
	v_object_t *first;
	v_object_t *last;
};

/* Functions */
int swap(vector_t *, size_t, size_t);
int sort(vector_t *, enum sort);
void *front(vector_t *);
void *back(vector_t *);
int clear(vector_t *);
int insert(vector_t *, size_t, void *);
int push_back(vector_t *, void *);
int erase(vector_t *, size_t);
int pop_back(vector_t *);
size_t get_size(vector_t *);
void *at(vector_t *, size_t);
v_object_t *get(vector_t *, size_t);
int empty(vector_t *);

/* Constructors */
vector_t *new_vector();
v_object_t *new_object(void *data);

#endif //VECTOR_VECTOR_H
