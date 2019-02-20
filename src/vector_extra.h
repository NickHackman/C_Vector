#ifndef VECTOR_EXTRA_H
#define VECTOR_EXTRA_H

#ifndef VECTOR_H
	#include "vector.h"
#endif

#include <string.h>

/*
 * Description: Inserts element at a specific location,
 *  	pushing all other values back.
 *
 * Type: Modifier (Insertion)
 *
 * Params:
 *
 * 	vector: the vector to modify
 *
 * 	location: location to insert at
 *
 * 	value: value to insert into vector
 *
 * Time Complexity: linear
 *
 * Memory:
 * 	Case of self->size == self->capacity:
 * 		self->capacity * 2
 *
 * Return: void
 */
#define vector_insert(vector, location, value) \
	if (!vector_size(vector) || location >= vector_size(vector)) { \
		vector_push_back(vector, value); \
	} else { \
		if ((vector_size(vector) + 1) >= vector_capacity(vector)) { \
			vector = __vector_alloc(vector, (vector) ? ((size_t)(2 * \
					vector_capacity(vector))) : ((size_t)12), sizeof(*(vector))); \
		} \
		unsigned char swap_temp[(signed)sizeof(*(vector))]; \
		unsigned char swap_temp1[(signed)sizeof(*(vector))]; \
		memcpy(swap_temp, &vector[location], sizeof(*(vector))); \
		vector[location] = value; \
		for (size_t i = location + 1; i < vector_size(vector) + 1; i++) { \
			memcpy(swap_temp1, &vector[i], sizeof(*(vector))); \
			memcpy(&vector[i], &swap_temp, sizeof(*(vector))); \
			memcpy(swap_temp, swap_temp1, sizeof(*(vector))); \
		} \
		__vector_set_size(vector, vector_size(vector) + 1); \
	}

/*
 * Description: Deletes an element at a specified location
 *
 * Type: Modifier (Deletion)
 *
 * Params:
 *
 * 	vector: the vector to modify
 *
 * 	location: location to remove
 *
 * Time Complexity: linear
 *
 * Memory:
 * if VECTOR_SHRINK_ON_REMOVE && size == capacity / 4
 * -((capacity / 2) * sizeof(*(vector)))
 * else
 * 0
 *
 * Logic:
 * if vector exists and location is less than size
 *  	go from location to last element and move the value to remove over (via swap)
 * pop_back
 *
 * Return: element removed
 */
#define vector_erase(vector, location) \
	({ if (vector && location < vector_size(vector)) { \
		unsigned char swap_temp[(signed)sizeof(*(vector))]; \
		for (size_t i = location; i < vector_size(vector) - 1; i++) { \
			memcpy(swap_temp, &vector[i], sizeof(*(vector))); \
			vector[i] = vector[i + 1]; \
	 		memcpy(&vector[i+1], &swap_temp, sizeof(*(vector))); \
		} \
	} \
	vector_pop_back(vector); })

/*
 * Description: Searches the vector, but only works for primitives
 *
 * Type: Search
 *
 * Params:
 *
 * 	vector: the vector to search
 *
 * 	value: value to search for
 *
 * Time Complexity: linear
 *
 * Memory:
 * 0
 *
 * Return: 1 if found, -1 if not found
 */
#define vector_find_basic(vector, value) ({\
	int found = -1; \
	if (sizeof(value) == sizeof(*(vector))) { \
		for (size_t i = 0; found == -1 && i < vector_size(vector); i++) { \
			if (vector[i] == value) { \
				found = 1; \
			} \
		} \
	} \
	found; })

/*
 * Description: Searches the vector, requires a function,
 * but can work for all values
 *
 * Type: Search
 *
 * Params:
 *
 * 	vector: the vector to search
 *
 * 	value: value to search for
 *
 * 	compare: returns 1 if the two void*s are equal
 *
 * Time Complexity: linear
 *
 * Memory:
 * 0
 *
 * Return: 1 if found, -1 if not found
 */
int vector_find_complex(void** vector, void* value, int compare(void*, void*)) {
	int found = -1;
	for (size_t i = 0; !found && i < vector_size(vector); i++) {
		if (compare(value, vector[i])) {
			found = 1;
		}
	}
	return found;
}

/*
 * Description: Searches the vector and returns the index, but only works on
 *  	primitives
 *
 * Type: Search
 *
 * Params:
 *
 * 	vector: the vector to search
 *
 * 	value: value to search for
 *
 * Time Complexity: linear
 *
 * Memory:
 * 0
 *
 * Return: a value < size if found otherwise size
 */
#define vector_find_index_basic(vector, value) ({\
	size_t index = vector_size(vector); \
	if (sizeof(value) == sizeof(*(vector))) { \
		for (size_t i = 0; index == vector_size(vector) && \
				i < vector_size(vector); i++) { \
			if (vector[i] == value) { \
				index = i; \
			} \
		} \
	} \
	index; })

/*
 * Description: Searches the vector, requires a function,
 *  	but can work for all values, and returns the index
 *
 * Type: Search
 *
 * Params:
 *
 * 	vector: the vector to search
 *
 * 	value: value to search for
 *
 * 	compare: returns 1 if the two void*s are equal
 *
 * Time Complexity: linear
 *
 * Memory:
 * 0
 *
 * Return: a value < size if found otherwise size
 */
size_t vector_find_index_complex(void** vector, void* value,
		int compare(void*, void*)) {
	size_t index = vector_size(vector);
	for (size_t i = 0; i < vector_size(vector); i++) {
		if (compare(vector[i], value)) {
			index = i;	
		}
	}
	return index;
}

/*
 * Description: Frees all values contained inside the vector
 *
 * Type: Modifier (free)
 *
 * Params:
 *
 * 	vector: the vector to search
 *
 * 	free_inner: frees an element in the vector
 *
 * Time Complexity: linear
 *
 * Memory:
 * frees all memory owned by vector
 *
 * Return: void
 */
void vector_free_all(void** vector, void free_inner(void*)) {
	for (size_t i = vector_size(vector) - 1; i > 0; i++) {
		free_inner(vector[i]);
	}
	vector_free(vector);
}

#endif // VECTOR_EXTRA_H
