/*
** EPITECH PROJECT, 2018
** vector
** File description:
** vector.c
*/

#include <stdlib.h>
#include <memory.h>
#include "../inc/vector.h"

size_t get_size(vector_t *vector)
{
	return (vector) ? (vector->size) : (0);
}

v_object_t *new_object(void *data)
{
	v_object_t *object = malloc(sizeof(v_object_t));

	if (!object)
		return (NULL);
	object->data = data;
	object->next = NULL;
	object->prev = NULL;
}

v_object_t *get(vector_t *vector, size_t pos)
{
	v_object_t *obj;

	if (!vector || vector->first == NULL)
		return (NULL);
	obj = vector->first;
	for (size_t i = 0; i < vector->size && i != pos; i++){
		obj = obj->next;
	}
	return (obj);
}

void *at(vector_t *vector, size_t pos)
{
	v_object_t *obj;

	if (!vector || vector->first == NULL)
		return (NULL);
	obj = vector->first;
	for (size_t i = 0; i < vector->size && i != pos; i++) {
		obj = obj->next;
	}
	return (obj) ? (obj->data) : (NULL);
}

int push_back(vector_t *vector, void *data)
{
	v_object_t *object = new_object(data);

	if (!vector)
		return (-1);
	if (vector->size == 0) {
		vector->first = object;
		vector->last = object;
	} else {
		vector->last->next = object;
		object->prev = vector->last;
		vector->last = object;
	}
	vector->size++;
}

int erase(vector_t *vector, size_t pos)
{
	v_object_t *obj;

	if (!vector || !vector->first)
		return (0);
	obj = vector->first;
	for (size_t i = 0; i < vector->size && i != pos; i++) {
		obj = obj->next;
	}
	if (!obj->prev) {
		if (obj->next) {
			vector->first = obj->next;
			obj->next->prev = NULL;
		} else {
			vector->first = NULL;
			vector->last = NULL;
		}
	} else if (!obj->next) {
		if (obj->prev) {
			vector->last = obj->prev;
			obj->prev->next = NULL;
		} else {
			vector->last = NULL;
			vector->first = NULL;
		}
	} else {
		obj->next->prev = obj->prev;
		obj->prev->next = obj->next;
	}
	free(obj);
	vector->size--;
	return (1);
}

int pop_back(vector_t *vector)
{
	return (erase(vector, vector->size - 1));
}

int empty(vector_t *vector)
{
	return (vector->size == 0) ? (1) : (-1);
}

int clear(vector_t *vector)
{
	v_object_t *obj;

	if (!vector || !vector->first) {
		return (-1);
	}
	obj = vector->first;
	for (size_t i = 0; i < vector->size; i++) {
		if (obj->next) {
			obj = obj->next;
		}
		if (i < (vector->size - 1) && obj->prev)
			free(obj->prev);
		else
			free(obj);
	}
	free(vector);
	return (1);
}

void *front(vector_t *vector)
{
	return (vector->at(vector, 0));
}

void *back(vector_t *vector)
{
	return (vector->at(vector, vector->size - 1));
}

int insert(vector_t *vector, size_t pos, void *data)
{
	v_object_t *obj;
	v_object_t *new_obj = new_object(data);

	if (!vector || !new_obj)
		return (-1);
	if (vector->size == 0)
		return (push_back(vector, data));
	obj = vector->first;
	for (size_t i = 0; i < vector->size && i != pos; i++) {
		obj = obj->next;
	}
	if (pos == 0) {
		obj->prev = new_obj;
		new_obj->next = obj;
		vector->first = new_obj;
	} else if (!obj->next) {
		obj->prev->next = new_obj;
		obj->prev = new_obj;
		new_obj->next = obj;
		vector->last = obj;
	} else {
		new_obj->prev = obj->prev;
		new_obj->prev->next = new_obj;
		new_obj->next = obj;
		obj->prev = new_obj;
	}
	vector->size++;
	return (1);
}

int swap(vector_t *vector, size_t pos1, size_t pos2)
{

	return (1);
}

int sort(vector_t *vector, enum sort sort)
{
	return (1);
}

vector_t *new_vector()
{
	vector_t *vector = malloc(sizeof(vector_t));

	if (!vector)
		return (NULL);
	vector->size = 0;
	vector->first = NULL;
	vector->last = NULL;
	vector->get_size = get_size;
	vector->push_back = push_back;
	vector->empty = empty;
	vector->at = at;
	vector->erase = erase;
	vector->pop_back = pop_back;
	vector->insert = insert;
	vector->clear = clear;
	vector->front = front;
	vector->back = back;
	vector->sort = sort;
	vector->swap = swap;
	vector->get = get;
	return (vector);
}
