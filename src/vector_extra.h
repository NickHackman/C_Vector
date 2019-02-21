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
 * Description: Frees all values contained inside the vector
 *
 * Type: Modifier (free)
 *
 * Params:
 *
 * 	vector: the vector to search
 *
 * 	free_inner: function that frees an element in the vector
 *
 * Time Complexity: linear
 *
 * Memory:
 * frees all memory owned by vector
 *
 * Return: void
 */
#define vector_free_all(vector, free_inner) \
	for (size_t i = 0; i < vector_size(vector); i++) { \
		(free_inner)(vector[i]); \
	} \
	vector_free(vector);

#endif // VECTOR_EXTRA_H
