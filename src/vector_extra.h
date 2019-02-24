/**************************************************************************************************
 * License: MIT                                                                                   *
 **************************************************************************************************
 * Copyright 2019 Scott Nicholas Hackman                                                          *
 *                                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining                          *
 * a copy of this software and associated documentation files (the "Software"),                   *
 * to deal in the Software without restriction, including without limitation                      *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,                       *
 * and/or sell copies of the Software, and to permit persons to whom the                          *
 * Software is furnished to do so, subject to the following conditions:                           *
 *                                                                                                *
 * The above copyright notice and this permission notice shall be included                        *
 * in all copies or substantial portions of the Software.                                         *
 *                                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS                        *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                    *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL                        *
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                     *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.        *
 **************************************************************************************************/

#ifndef VECTOR_EXTRA_H
#define VECTOR_EXTRA_H

#ifndef VECTOR_H
	#include "vector.h"
#endif

#include <string.h> // memcpy, strlen

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
 * 		self->capacity * 2 + 2 * sizeof(size_t)
 *
 * Return: void
 */
#define vector_insert(vector, location, value) \
	if (!vector_size((vector)) || (location) >= vector_size((vector))) { \
		vector_push_back((vector), (value)); \
	} else { \
		if ((vector_size((vector)) + 1) >= vector_capacity((vector))) { \
			vector = __vector_alloc((vector), (vector) ? ((size_t)(2 * \
					vector_capacity((vector))) : ((size_t)12), sizeof(*(vector))); \
		} \
		unsigned char __swap_temp[(signed)sizeof(*(vector))]; \
		unsigned char __swap_temp1[(signed)sizeof(*(vector))]; \
		memcpy(__swap_temp, &vector[location], sizeof(*(vector))); \
		vector[(location)] = (value); \
		for (size_t __i = (location) + 1; __i < vector_size((vector)) + 1; __i++) { \
			memcpy(__swap_temp1, &(vector)[__i], sizeof(*(vector))); \
			memcpy(&(vector)[__i], &__swap_temp, sizeof(*(vector))); \
			memcpy(__swap_temp, __swap_temp1, sizeof(*(vector))); \
		} \
		__vector_set_size((vector), vector_size((vector)) + 1); \
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
	({  \
		if ((vector) && (location) < vector_size((vector))) { \
			unsigned char __swap_temp[(signed)sizeof(*(vector))]; \
			for (size_t __i = (location); __i < vector_size((vector)) - 1; __i++) { \
				memcpy(__swap_temp, &(vector)[__i], sizeof(*(vector))); \
				(vector)[__i] = (vector)[__i + 1]; \
	 			memcpy(&(vector)[__i+1], &__swap_temp, sizeof(*(vector))); \
			} \
		} \
		vector_pop_back((vector)); \
	})

/*
 * Description: Filters all values in vector,
 * 		all removed values are put into removed
 *
 * Type: Modifier (Insertion)
 *
 * Params:
 *
 * 	vector: vector to filter
 *
 * 	filter: function that filters, return >= 1 to keep value
 *
 * 	removed: filtered out items are put in order into this vector
 *
 * Time Complexity: linear in size of src
 *
 * Memory:
 * (dest_capacity + src_capacity * sizeof(*(src)))
 *
 * Return: void
 */
#define vector_filter(vector, filter, removed) \
	if (vector_capacity((removed)) <= vector_capacity((vector))) { \
		(removed) = __vector_alloc((removed), vector_capacity((vector)) + \
				vector_capacity((removed), sizeof(*(vector))); \
	} \
	size_t __move_count = 0; \
	for (size_t __i = 0; __i < vector_size((vector)); __i++) { \
		if (filter(vector[__i])) { \
			(vector)[__i - __move_count] = (vector)[__i]; \
		} else { \
			__move_count++; \
			vector_push_back((removed), (vector)[__i]); \
		} \
	} \
	__vector_set_size((vector), vector_size((vector)) - __move_count);

/*
 * Description: Appends all values in src to dest, frees src's array
 *
 * Type: Modifier (Insertion)
 *
 * Params:
 *
 * 	dest: destination to put values into
 *
 * 	src: values to move, array is freed, but not internal data
 *
 * Time Complexity: linear in size of src
 *
 * Memory:
 * (dest_capacity + src_capacity * sizeof(*(src)))
 *
 * Return: void
 */
#define vector_append_vector(dest, src) \
	if (vector_capacity((dest)) <= vector_capacity((src))) { \
		dest = __vector_alloc(dest, vector_capacity((src)) + \
				vector_capacity((dest)), sizeof(*(src))); \
	} \
	for (size_t __i = 0; __i < vector_size((src)); __i++) { \
		vector_push_back((dest), (src)[__i]); \
	} \
	vector_free((src));

/*
 * Description: Appends n values in src to dest, doesn't free src
 *
 * Type: Modifier (Insertion)
 *
 * Params:
 *
 * 	dest: destination to put values into
 *
 * 	src: values to move, doesn't free
 *
 * 	n: values to move
 *
 * Time Complexity: linear (n)
 *
 * Memory:
 * (dest_capacity + src_capacity * sizeof(*(src)))
 *
 * Return: void
 */
#define vector_append_array(dest, src, n) \
	if (vector_capacity(dest) <= (n)) { \
		(dest) = __vector_alloc((dest), 2 * (vector_capacity((dest)) + (n)), \
				sizeof(*(src))); \
	} \
	for (size_t __i = 0; __i < (n); __i++) { \
		vector_push_back((dest), (src)[__i]); \
	}

/*
 * Description: Frees all values contained inside the vector
 *
 * Type: Modifier (free)
 *
 * Params:
 *
 * 	vector: the vector to free
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
	for (size_t __i = 0; __i < vector_size((vector)); __i++) { \
		(free_inner)((vector)[__i]); \
	} \
	vector_free((vector));

/*
 * Description: Searches for a value, using the function equals and returns
 * 		1 if found or -1 if not
 *
 * Type: Search
 *
 * Params:
 *
 * 	vector: the vector to search
 *
 * 	value: value to search for
 *
 * 	equals: function that compares TYPE and TYPE and implements ==
 *
 * Time Complexity: linear
 *
 * Memory:
 * 0
 *
 * Return: 1 if found or -1 if not found (int)
 */
#define vector_find(vector, value, equals) \
	({ \
		int __found = -1; \
		for (size_t __i = 0; __found == -1 && __i < vector_size((vector)); __i++) { \
			if ((equals)((value), (vector)[__i])) { \
				__found = 1; \
			} \
		} \
		__found; \
	 })

/*
 * Description: Searches for a value, using the function equals and returns
 * 		the index of the first occurence
 *
 * Type: Search
 *
 * Params:
 *
 * 	vector: the vector to search
 *
 * 	value: value to search for
 *
 * 	equals: function that compares TYPE and TYPE and implements ==
 *
 * Time Complexity: linear
 *
 * Memory:
 * 0
 *
 * Return: index (size_t)
 */
#define vector_find_index(vector, value, equals) \
	({ \
		size_t __found = vector_size(vector); \
		for (size_t __i = 0; __found == vector_size((vector)) && \
				__i < vector_size((vector)); __i++) { \
			if ((equals)((vector)[__i], (value))) { \
				__found = __i; \
			} \
		} \
		__found; \
	 })

/*
 * Description: Converts vector to a string, with provided to_string
 * 		for the type, output: [a, b, c, d, ..., z]
 *
 * Type: To_string
 *
 * Params:
 *
 * 	vector: the vector to get the string equivalent of
 *
 * 	to_string: converts a value in vector to a string
 *
 * Time Complexity: linear
 *
 * Memory:
 * allocates a new vector with capacity enough to hold entire 
 * 		string equivalent
 *
 * Return: a char* vector, not an array. Must be freed
 */
#define vector_to_string(vector, to_string) \
	({ \
		char* __string = __vector_alloc(NULL, vector_size((vector)) * 5 + 4, \
				sizeof(char)); \
		vector_push_back(__string, '['); \
		for (size_t __i = 0; __i < vector_size((vector)); __i++) { \
			char* __str = to_string((vector)[__i]); \
			size_t __len = strlen(__str); \
			for (size_t __j = 0; __j < __len; __j++) { \
				vector_push_back(__string, __str[__j]); \
			} \
			free(__str); \
			if (__i < (vector_size((vector)) - 1)) { \
				vector_push_back(__string, ','); \
				vector_push_back(__string, ' '); \
			} \
		} \
		vector_push_back(__string, ']'); \
		vector_push_back(__string, '\0'); \
		__string; \
	})

/*
 * Description: Reverses the vector
 *
 * Type: Modifier
 *
 * Params:
 *
 * 	vector: the vector to reverse/flip
 *
 * Time Complexity: linear
 *
 * Memory:
 *
 * 0
 *
 * Return: void
 */
#define vector_reverse(vector) \
	unsigned char swap_temp[(signed)sizeof(*(vector))]; \
	for (size_t i = 0; i < (vector_size(vector) / 2); i++) { \
		memcpy(swap_temp, &vector[i], sizeof(*(vector))); \
		vector[i] = vector[vector_size(vector) - (1 + i)]; \
		memcpy(&vector[vector_size(vector) - (1 + i)], &swap_temp, sizeof(*(vector))); \
	}

#endif // VECTOR_EXTRA_H
