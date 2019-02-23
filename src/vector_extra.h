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
	({  \
		if (vector && location < vector_size(vector)) { \
			unsigned char swap_temp[(signed)sizeof(*(vector))]; \
			for (size_t i = location; i < vector_size(vector) - 1; i++) { \
				memcpy(swap_temp, &vector[i], sizeof(*(vector))); \
				vector[i] = vector[i + 1]; \
	 			memcpy(&vector[i+1], &swap_temp, sizeof(*(vector))); \
			} \
		} \
		vector_pop_back(vector); \
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
	if (vector_capacity(removed) <= vector_capacity(vector)) { \
		removed = __vector_alloc(removed, vector_capacity(vector) + \
				vector_capacity(removed), sizeof(*(vector))); \
	} \
	size_t move_count = 0; \
	for (size_t i = 0; i < vector_size(vector); i++) { \
		if (filter(vector[i])) { \
			vector[i - move_count] = vector[i]; \
		} else { \
			move_count++; \
			vector_push_back(removed, vector[i]); \
		} \
	} \
	__vector_set_size(vector, vector_size(vector) - move_count);

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
	if (vector_capacity(dest) <= vector_capacity(src)) { \
		dest = __vector_alloc(dest, vector_capacity(src) + \
				vector_capacity(dest), sizeof(*(src))); \
	} \
	for (size_t i = 0; i < vector_size(src); i++) { \
		vector_push_back(dest, src[i]); \
	} \
	vector_free(src);

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
	if (vector_capacity(dest) <= n) { \
		dest = __vector_alloc(dest, 2 * (vector_capacity(dest) + n), \
				sizeof(*(src))); \
	} \
	for (size_t i = 0; i < n; i++) { \
		vector_push_back(dest, src[i]); \
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
	for (size_t i = 0; i < vector_size(vector); i++) { \
		(free_inner)(vector[i]); \
	} \
	vector_free(vector);

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
		int found = -1; \
		for (size_t i = 0; found == -1 && i < vector_size(vector); i++) { \
			if (equals(vector[i], value)) { \
				found = 1; \
			} \
		} \
		found; \
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
		size_t found = vector_size(vector); \
		for (size_t i = 0; found == vector_size(vector) && \
				i < vector_size(vector); i++) { \
			if (equals(vector[i], value)) { \
				found = i; \
			} \
		} \
		found; \
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
		char* string = __vector_alloc(NULL, vector_size(vector) * 5 + 4, \
				sizeof(char)); \
		vector_push_back(string, '['); \
		for (size_t i = 0; i < vector_size(vector); i++) { \
			char* str = to_string(vector[i]); \
			size_t len = strlen(str); \
			for (size_t j = 0; j < len; j++) { \
				vector_push_back(string, str[j]); \
			} \
			free(str); \
			if (i < (vector_size(vector) - 1)) { \
				vector_push_back(string, ','); \
				vector_push_back(string, ' '); \
			} \
		} \
		vector_push_back(string, ']'); \
		vector_push_back(string, '\0'); \
		string; \
	})

#endif // VECTOR_EXTRA_H
